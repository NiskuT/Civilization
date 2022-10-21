/***************************************************************************
         generate_code_cpp.c  -  Function that generates C++ code
                             -------------------
    begin                : Sat Dec 16 2000
    copyright            : (C) 2000-2001 by Javier O'Hara
                           (C) 2002-2014 by Oliver Kellogg
    email                : joh314@users.sourceforge.net
                           okellogg@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* NB: If you use CORBA stereotypes, you will need the file p_orb.h
   found in the runtime/cpp directory.  */

#include "dia2code.h"
#include "decls.h"
#include "includes.h"

#define SPEC_EXT "h"
#define BODY_EXT "cpp"

#define eq  !strcmp

static batch *gb;   /* The current batch being processed.  */

/* Utilities.  */

int is_scalar_type(char* name) {
    if (strcmp(name,"int") == 0) {
        return 1;
    }
    if (strcmp(name,"char") == 0) {
        return 1;
    }
    if (strcmp(name,"float") == 0) {
        return 1;
    }
    if (strcmp(name,"bool") == 0) {
        return 1;
    }
    if (strcmp(name,"unsigned int") == 0) {
        return 1;
    }
    if (strcmp(name,"unsigned char") == 0) {
        return 1;
    }
    if (strcmp(name,"double") == 0) {
        return 1;
    }    
    umlclassnode *ref = find_by_name (gb->classlist, name);
    if (ref && is_enum_stereo(ref->key->stereotype)) {
        return 1;
    }
    return 0;
}

static void
check_umlattr (umlattribute *u, char *typename)
{
    /* Check settings that don't make sense for C++ generation.  */
    if (u->visibility == '1')
        fprintf (stderr, "%s/%s: ignoring non-visibility\n", typename, u->name);
    if (u->isstatic)
        fprintf (stderr, "%s/%s: ignoring staticness\n", typename, u->name);
}

static char *
subst (char *str, char search, char replace)
{
    char *p;
    while ((p = strchr (str, search)) != NULL)
        *p = replace;
    return str;
}

static char *
nospc (char *str)
{
    return subst (str, ' ', '_');
}

static int
pass_by_reference (umlclass *cl)
{
    char *st;
    if (cl == NULL)
        return 0;
    st = cl->stereotype;
    if (strlen (st) == 0)
        return 1;
    if (is_typedef_stereo (st)) {
        umlattrlist umla = cl->attributes;
        umlclassnode *ref = find_by_name (gb->classlist, cl->name);
        if (ref == NULL)
            return 0;
        return pass_by_reference (ref->key);
    }
    return (!is_const_stereo (st) &&
            !is_enum_stereo (st));
}

static int
is_oo_class (umlclass *cl)
{
    char *st;
    if (cl == NULL)
        return 0;
    st = cl->stereotype;
    if (strlen (st) == 0)
        return 1;
    return (!is_const_stereo (st) &&
            !is_typedef_stereo (st) &&
            !is_enum_stereo (st) &&
            !is_struct_stereo (st) &&
            !eq (st, "CORBAUnion") &&
            !eq (st, "CORBAException"));
}

static int
has_oo_class (declaration *d)
{
    while (d != NULL) {
        if (d->decl_kind == dk_module) {
            if (has_oo_class (d->u.this_module->contents))
                return 1;
        } else {         /* dk_class */
            if (is_oo_class (d->u.this_class->key))
                return 1;
        }
        d = d->next;
    }
    return 0;
}

static char *
cppname (char *name)
{
    static char buf[SMALL_BUFFER];
    if (use_corba) {
        if (eq (name, "boolean") ||
            eq (name, "char") ||
            eq (name, "octet") ||
            eq (name, "short") ||
            eq (name, "long") ||
            eq (name, "float") ||
            eq (name, "double") ||
            eq (name, "string") ||
            eq (name, "any")) {
            sprintf (buf, "CORBA::%s", strtoupperfirst (name));
        } else if (eq (name, "long long")) {
            sprintf (buf, "CORBA::LongLong");
        } else if (eq (name, "unsigned short")) {
            sprintf (buf, "CORBA::UShort");
        } else if (eq (name, "unsigned long")) {
            sprintf (buf, "CORBA::ULong");
        } else if (eq (name, "unsigned long long")) {
            sprintf (buf, "CORBA::ULongLong");
        } else {
            strcpy (buf, name);
        }
    } else {
        strcpy (buf, name);
    }
    return buf;
}


static char *
fqname (umlclassnode *node, int use_ref_type)
{
    static char buf[BIG_BUFFER];

    buf[0] = '\0';
    if (node == NULL)
        return buf;
    if (node->key->package != NULL) {
        umlpackagelist pkglist = make_package_list (node->key->package);
        while (pkglist != NULL) {
            strcat (buf, pkglist->key->name);
            strcat (buf, "::");
            pkglist = pkglist->next;
        }
    }
    strcat (buf, node->key->name);
    if (use_ref_type)
        strcat (buf, "*");
    return buf;
}

static void
check_visibility (int *curr_vis, int new_vis)
{
    if (*curr_vis == new_vis)
        return;
    indentlevel--;
    switch (new_vis) {
      case '0':
        print ("public:\n");
        break;
      case '1':
        print ("private:\n");
        break;
      case '2':
        print ("protected:\n");
        break;
    }
    *curr_vis = new_vis;
    indentlevel++;
}

static void 
add_setter_getter(umlclassnode *node, char* typename,char* varname)
{
    int foundGetter = 0;
    int foundSetter = 0;
    char* tmp = strtoupperfirst(varname);
    char getter[0x100];
    strcpy(getter,"get");
    strcat(getter,tmp);
    char setter[0x100];
    strcpy(setter,"set");
    strcat(setter,tmp);
    free(tmp);
    if (node->key->operations != NULL) {
        umloplist umlo = node->key->operations;
        while (umlo != NULL) {
            if (strcmp(umlo->key.attr.name,getter) == 0) {
                foundGetter = 1;
            }                        
            if (strcmp(umlo->key.attr.name,setter) == 0) {
                foundSetter = 1;
            }                        
            umlo = umlo->next;
        }
    }

    if (!foundGetter) {
        print ("");
        if (is_scalar_type(typename) || typename[strlen(typename)-1] == '&') {
            emit ("%s %s() const;\n", typename, getter);
        }
        else {
            emit ("const %s& %s() const;\n", typename, getter);
        }
    }
    if (!foundSetter && strstr(typename,"const ") != typename) {
        print ("");
        if (is_scalar_type(typename)) {
            emit ("void %s(%s %s);\n", setter, typename, varname);
        }
        else {
            emit ("void %s(const %s& %s);\n", setter, typename, varname);
        }
    }
    
}

static void
gen_class (umlclassnode *node)
{
    char *name = node->key->name;
    char *stype = node->key->stereotype;
    int is_valuetype = 0;

    if (strlen (stype) > 0) {
        print ("// %s\n", stype);
        is_valuetype = eq (stype, "CORBAValue");
    }

    print("/// class %s - %s\n", name, node->key->comment);

    if (node->key->templates != NULL) {
        umltemplatelist template = node->key->templates;
        if (is_valuetype) {
            fprintf (stderr, "CORBAValue %s: template ignored\n", name);
        } else {
            print ("template <");
            while (template != NULL) {
                print ("%s %s", template->key.type, template->key.name);
                template = template->next;
                if (template != NULL)
                    emit (", ");
            }
            emit (">\n");
        }
    }

    print ("class %s", name);
    if (node->parents != NULL) {
        umlclasslist parent = node->parents;
        emit (" : ");
        while (parent != NULL) {
            emit ("public %s", fqname (parent, 0));
            parent = parent->next;
            if (parent != NULL)
                emit (", ");
        }
    } else if (is_valuetype) {
        emit (" : CORBA::ValueBase");
    }
    emit (" {\n");
    indentlevel++;

    if (node->associations != NULL) {
        umlassoclist assoc = node->associations;
        print ("// Associations\n");
        while (assoc != NULL) {
            umlclassnode *ref;
            if (assoc->name[0] != '\0')
            {
                ref = find_by_name (gb->classlist, assoc->key->name);
                print ("");
                if (ref != NULL)
                    emit ("%s", fqname (ref, !assoc->composite));
                else
                    emit ("%s", cppname (assoc->key->name));
                emit (" %s;\n", assoc->name);
            }
            assoc = assoc->next;
        }
    }

    if (node->key->attributes != NULL) {
        umlattrlist umla = node->key->attributes;
        if (is_valuetype) {
            print ("// Public state members\n");
            indentlevel--;
            print ("public:\n");
            indentlevel++;
            while (umla != NULL) {
                char *member = umla->key.name;
                umlclassnode *ref;
                if (umla->key.visibility != '0') {
                    umla = umla->next;
                    continue;
                }
                print ("");
                if (umla->key.isstatic) {
                    fprintf (stderr, "CORBAValue %s/%s: static not supported\n",
                                     name, member);
                }
                ref = find_by_name (gb->classlist, umla->key.type);
                if (ref != NULL)
                    eboth ("%s", fqname (ref, 1));
                else
                    eboth ("%s", cppname (umla->key.type));
                emit (" %s () { return _%s; }\n", member, member);
                print ("void %s (", member);
                if (ref != NULL) {
                    int by_ref = pass_by_reference (ref->key);
                    if (by_ref)
                        emit ("const ");
                    emit ("%s", fqname (ref, 1));
                    if (by_ref)
                        emit ("&");
                } else {
                    emit ("%s", cppname (umla->key.type));
                }
                emit (" value_) { _%s = value_; }\n");
                umla = umla->next;
            }
        } else {
            int tmpv = -1;
            print ("// Attributes\n");
            while (umla != NULL) {
                check_visibility (&tmpv, umla->key.visibility);
                if (strlen(umla->key.comment)) {
                    print("/// %s\n", umla->key.comment);
                }
                print ("");
                if (umla->key.isstatic) {
                    emit ("static ");
                }
                emit ("%s %s", umla->key.type, umla->key.name);
                if (strlen (umla->key.value) > 0)
                    print (" = %s", umla->key.value);
                emit (";\n");
                umla = umla->next;
            }
        }
    }

    if (node->key->operations != NULL) {
        umloplist umlo = node->key->operations;
        int tmpv = -1;
        print ("// Operations\n");
        if (is_valuetype) {
            indentlevel--;
            print ("public:\n");
            indentlevel++;
        }
        while (umlo != NULL) {
            umlattrlist tmpa = umlo->key.parameters;
            if (is_valuetype) {
                if (umlo->key.attr.visibility != '0')
                    fprintf (stderr, "CORBAValue %s/%s: must be public\n",
                                     name, umlo->key.attr.name);
            } else {
                check_visibility (&tmpv, umlo->key.attr.visibility);
            }

            /* print comments on operation */
            if (strlen(umlo->key.attr.comment)) {
                print("/// %s\n", umlo->key.attr.comment);
                tmpa = umlo->key.parameters;
                while (tmpa != NULL) {
                     print("/// @param %s\t\t(%s) %s\n",
                           tmpa->key.name,
                           kind_str(tmpa->key.kind),
                           tmpa->key.comment);
                           tmpa = tmpa->next;
                }
            }
            /* print operation */
            print ("");
            if (umlo->key.attr.inheritance_type == '1') {
                emit ("virtual ");
            }
            else if (umlo->key.attr.isabstract || is_valuetype) {
                emit ("virtual ");
                umlo->key.attr.value[0] = '0';
            }
            if (umlo->key.attr.isstatic) {
                if (is_valuetype)
                    fprintf (stderr, "CORBAValue %s/%s: static not supported\n",
                                     name, umlo->key.attr.name);
                else
                    emit ("static ");
            }
            if (strlen (umlo->key.attr.type) > 0) {
                emit ("%s ", cppname (umlo->key.attr.type));
            }
            emit ("%s (", umlo->key.attr.name);
            tmpa = umlo->key.parameters;
            while (tmpa != NULL) {
                emit ("%s %s", tmpa->key.type, tmpa->key.name);
                if (tmpa->key.value[0] != 0) {
                    if (is_valuetype)
                        fprintf (stderr, "CORBAValue %s/%s: param default "
                                 "not supported\n", name, umlo->key.attr.name);
                    else
                       emit (" = %s", tmpa->key.value);
                }
                tmpa = tmpa->next;
                if (tmpa != NULL) {
                    emit (", ");
                }
            }
            emit (")");
            if (umlo->key.attr.isconstant) {
                emit (" const");
            }
            if (umlo->key.attr.value[0]) {
                // virtual
                if ((umlo->key.attr.isabstract || is_valuetype) &&
                    umlo->key.attr.name[0] != '~')
                    emit (" = %s", umlo->key.attr.value);
            }
            emit (";\n");
            umlo = umlo->next;
        }
    }

    print ("// Setters and Getters\n");
    if (node->associations != NULL) {
        umlassoclist assoc = node->associations;
        while (assoc != NULL) {
            umlclassnode *ref;
            if (assoc->name[0] != '\0')
            {
                add_setter_getter(node,assoc->key->name,assoc->name);
            }
            assoc = assoc->next;
        }
    }
    if (node->key->attributes != NULL) {
        umlattrlist umla = node->key->attributes;
        if (!is_valuetype) {
            while (umla != NULL) {
                if (umla->key.visibility != '2') {
                    umla = umla->next;
                    continue;
                }
                if (umla->key.isstatic) {
                    umla = umla->next;
                    continue;
                }
                add_setter_getter(node,umla->key.type,umla->key.name);
                
                umla = umla->next;
            }
        }
    }
    
    
    if (node->key->attributes != NULL && is_valuetype) {
        umlattrlist umla = node->key->attributes;
        emit ("\n");
        indentlevel--;
        print ("private:  // State member implementation\n");
        indentlevel++;
        while (umla != NULL) {
            umlclassnode *ref = find_by_name (gb->classlist, umla->key.type);
            print ("");
            if (ref != NULL) {
                emit ("%s", fqname (ref, is_oo_class (ref->key)));
                /*
                 * FIXME: Find a better way to decide whether to use
                 * a pointer.
                */
            } else
                emit ("%s", cppname (umla->key.type));
            emit (" _%s;\n", umla->key.name);
            umla = umla->next;
        }
    }

    indentlevel--;
    print ("};\n\n");
}

static void
gen_decl (declaration *d)
{
    char *name;
    char *stype;
    umlclassnode *node;
    umlattrlist umla;

    if (d == NULL)
        return;

    if (d->decl_kind == dk_module) {
#ifdef NO_NAMESPACE_SINGLE_FILE
        char* nsname = d->u.this_module->pkg->name;
        d = d->u.this_module->contents;
        while (d != NULL) {
            if (d->decl_kind == dk_module) {
                name = d->u.this_module->pkg->name;
                fprintf (stderr, "%s::%s: Nested namespaces are not supported\n", nsname, name);
                continue;
            }
            name = d->u.this_class->key->name;
            print ("#include \"%s/%s.%s\"\n", nsname, name, file_ext);
            d = d->next;
        }
#else
        name = d->u.this_module->pkg->name;
        print ("namespace %s {\n\n", name);
        indentlevel++;
        d = d->u.this_module->contents;
        while (d != NULL) {
            gen_decl (d);
            d = d->next;
        }
        indentlevel--;
        print ("};\n\n", name);
#endif
        return;
    }

    node = d->u.this_class;
    stype = node->key->stereotype;
    name = node->key->name;
    umla = node->key->attributes;

    if (strlen (stype) == 0) {
        gen_class (node);
        return;
    }

    if (eq (stype, "CORBANative")) {
        print ("// CORBANative: %s \n\n", name);

    } else if (is_const_stereo (stype)) {
        if (umla == NULL) {
            fprintf (stderr, "Error: first attribute not set at %s\n", name);
            exit (1);
        }
        if (strlen (umla->key.name) > 0)
            fprintf (stderr, "Warning: ignoring attribute name at %s\n", name);

        print ("const %s %s = %s;\n\n", cppname (umla->key.type), name,
                                                 umla->key.value);

    } else if (is_enum_stereo (stype)) {
        print ("enum %s {\n", name);
        indentlevel++;
        while (umla != NULL) {
            char *literal = umla->key.name;
            check_umlattr (&umla->key, name);
            if (strlen (umla->key.type) > 0)
                fprintf (stderr, "%s/%s: ignoring type\n", name, literal);
            print ("%s", literal);
            if (strlen (umla->key.value) > 0)
                print (" = %s", umla->key.value);
            if (umla->next)
                emit (",");
            emit ("\n");
            umla = umla->next;
        }
        indentlevel--;
        print ("};\n\n");

    } else if (is_struct_stereo (stype)) {
        print ("struct %s {\n", name);
        indentlevel++;
        while (umla != NULL) {
            check_umlattr (&umla->key, name);
            print ("%s %s", cppname (umla->key.type), umla->key.name);
            if (strlen (umla->key.value) > 0)
                print (" = %s", umla->key.value);
            emit (";\n");
            umla = umla->next;
        }
        indentlevel--;
        print ("};\n\n");

    } else if (eq (stype, "CORBAException")) {
        fprintf (stderr, "%s: CORBAException not yet implemented\n", name);

    } else if (eq (stype, "CORBAUnion")) {
        umlattrnode *sw = umla;
        if (sw == NULL) {
            fprintf (stderr, "Error: attributes not set at union %s\n", name);
            exit (1);
        }
        fprintf (stderr, "%s: CORBAUnion not yet fully implemented\n", name);
        print ("class %s {  // CORBAUnion\n", name);
        print ("public:\n", name);
        indentlevel++;
        print ("%s _d();  // body TBD\n\n", umla->key.type);
        umla = umla->next;
        while (umla != NULL) {
            check_umlattr (&umla->key, name);
            print ("%s %s ();  // body TBD\n",
                   cppname (umla->key.type), umla->key.name);
            print ("void %s (%s _value);  // body TBD\n\n", umla->key.name,
                   cppname (umla->key.type));
            umla = umla->next;
        }
        indentlevel--;
        print ("};\n\n");

    } else if (is_typedef_stereo (stype)) {
        /* Conventions for CORBATypedef:
           The first (and only) attribute contains the following:
           Name:   Empty - the name is taken from the class.
           Type:   Name of the original type which is typedefed.
           Value:  Optionally contains array dimension(s) of the typedef.
                   These dimensions are given in square brackets, e.g.
                   [3][10]
         */
        if (umla == NULL) {
            fprintf (stderr, "Error: first attribute (impl type) not set "
                             "at typedef %s\n", name);
            exit (1);
        }
        if (strlen (umla->key.name) > 0)  {
            fprintf (stderr, "Warning: typedef %s: ignoring name field "
                        "in implementation type attribute\n", name);
        }
        print ("typedef %s %s%s;\n\n", cppname (umla->key.type), name,
                                                umla->key.value);
    } else {
        gen_class (node);
    }
}

struct stdlib_includes {
   int string;
   int stdint;
   int stdlib;
   int vector;
   int memory;
   int limits;
   int map;
   int unordered_map;
   int set;
   int list;
   int unordered_set;
   int stack;
   int queue;
   int function;
   int array;   
   int thread;
   int mutex;
   int random;
   int sfmlGraphics;
   int jsoncpp;
};

void print_include_stdlib(struct stdlib_includes* si,char* name) {
    if ( strlen(name) > 0 ) {
       if (!si->stdint 
       && (strstr(name,"int8_t")
       ||  strstr(name,"uint8_t")
       ||  strstr(name,"int16_t")
       ||  strstr(name,"uint16_t")
       ||  strstr(name,"int32_t")
       ||  strstr(name,"uint32_t")
       ||  strstr(name,"int64_t")
       ||  strstr(name,"uint64_t"))) {
           print ("#include <stdint.h>\n");
           si->stdint = 1;
       }
       if (!si->stdlib && strstr(name,"size_t")) {
           print ("#include <stdlib.h>\n");
           si->stdlib = 1;
       }
       if (!si->string && strstr(name,"std::string")) {
           print ("#include <string>\n");
           si->string = 1;
       }
       if (!si->array && strstr(name,"std::array")) {
           print ("#include <array>\n");
           si->array = 1;
       }
       if (!si->vector && strstr(name,"std::vector")) {
           print ("#include <vector>\n");
           si->vector = 1;
       }
       if (!si->map && strstr(name,"std::map")) {
           print ("#include <map>\n");
           si->map = 1;
       }
        if (!si->function && strstr(name,"std::function")) {
            print ("#include <functional>\n");
            si->function = 1;
        }
       if (!si->set && strstr(name,"std::set")) {
           print ("#include <set>\n");
           si->set = 1;
       }
       if (!si->limits && strstr(name,"std::numeric_limits")) {
           print ("#include <limits>\n");
           si->limits = 1;
       }
       if (!si->list && strstr(name,"std::list")) {
           print ("#include <list>\n");
           si->list = 1;
       }
       if (!si->stack && strstr(name,"std::stack")) {
           print ("#include <stack>\n");
           si->stack = 1;
       }
       if (!si->mutex && strstr(name,"std::mutex")) {
           print ("#include <mutex>\n");
           si->mutex = 1;
       }
       if (!si->thread && strstr(name,"std::thread")) {
           print ("#include <thread>\n");
           si->thread = 1;
       }
       if (!si->queue
       && (strstr(name,"std::queue")
       ||  strstr(name,"std::priority_queue"))) {
           print ("#include <queue>\n");
           si->queue = 1;
       }
       if (!si->unordered_map && strstr(name,"std::unordered_map")) {
           print ("#include <unordered_map>\n");
           si->unordered_map = 1;
       }
       if (!si->unordered_set && strstr(name,"std::unordered_set")) {
           print ("#include <unordered_set>\n");
           si->unordered_set = 1;
       }
       if (!si->memory 
       && (strstr(name,"std::unique_ptr")
       ||  strstr(name,"std::shared_ptr")
       ||  strstr(name,"std::weak_ptr"))) {
           print ("#include <memory>\n");
           si->memory = 1;
       }
       if (!si->random 
       && (strstr(name,"std::mt19937")
       ||  strstr(name,"std::random_device")
       ||  strstr(name,"std::uniform_int_distribution"))) {
           print ("#include <random>\n");
           si->random = 1;
       }
       if (!si->sfmlGraphics 
       && (strstr(name,"sf::RenderWindow")
       ||  strstr(name,"sf::VertexArray")
       ||  strstr(name,"sf::Texture"))) {
           print ("#include <SFML/Graphics.hpp>\n");
           si->sfmlGraphics = 1;
       }       
       if (!si->jsoncpp
       && (strstr(name,"Json::") == name)) {
           print ("#include <json/json.h>\n");
           si->jsoncpp = 1;
       }       
    }
}

void
gen_namespace(batch *b, declaration *nsd) {
    char * nsname = nsd->u.this_module->pkg->name;
    declaration* d = nsd->u.this_module->contents;
    while (d != NULL) {  // For each class in namespace
        char *name, *tmpname;
        char filename[BIG_BUFFER];

        if (d->decl_kind == dk_module) {
            name = d->u.this_module->pkg->name;
            fprintf(stderr, "%s::%s: Nested namespaces are not supported\n", nsname, name);
            continue;
        } else { /* dk_class */
            name = d->u.this_class->key->name;
        }
        
        /////////////////////////////////////
        // Header file
        
#ifdef ENABLE_FILE_UPDATE_ON_CHANGE
        sprintf(filename, "%s/%s.%s~", nsname, name, file_ext);
#else
        sprintf(filename, "%s/%s.%s", nsname, name, file_ext);
        printf("Create '%s'\n",filename);
#endif
#if VERBOSE_LEVEL >= 1
        printf("\ngen_namespace(): generate file '%s'\n", filename);
#endif

        spec = open_outfile(filename, b);
        if (spec == NULL) {
            d = d->next;
            continue;
        }

        print("// Generated by dia2code\n");
        char * tmpnsname = strtoupper(nsname);
        tmpname = strtoupper(name);
        print("#ifndef %s__%s__H\n", tmpnsname, tmpname);
        print("#define %s__%s__H\n\n", tmpnsname, tmpname);

        // Find STL classes
        if (d->decl_kind != dk_module) {

            struct stdlib_includes si;
            memset(&si, 0, sizeof (struct stdlib_includes));

            // Parents
            if (d->u.this_class->parents != NULL) {
                umlclasslist parent = d->u.this_class->parents;
                while (parent != NULL) {
                    print_include_stdlib(&si, fqname (parent, 0));
                    parent = parent->next;
                }
            }

            // Attributes
            umlattrlist umla = d->u.this_class->key->attributes;
            while (umla != NULL) {
                print_include_stdlib(&si, umla->key.type);
                if (umla->key.value) {
                    print_include_stdlib(&si, umla->key.value);
                }
                umla = umla->next;
            }
            // Methods
            umloplist umlo = d->u.this_class->key->operations;
            while (umlo != NULL) {
                print_include_stdlib(&si, umlo->key.attr.type);
                umlattrlist tmpa = umlo->key.parameters;
                while (tmpa != NULL) {
                    print_include_stdlib(&si, tmpa->key.type);
                    if (tmpa->key.value) {
                        print_include_stdlib(&si, tmpa->key.value);
                    }
                    tmpa = tmpa->next;
                }
                umlo = umlo->next;
            }


            print("\n");
        }


        includes = NULL;
        determine_referenced_classes(d, b);
        if (includes) {
            namelist incfile = includes;
            char* curnsname = NULL;
            while (incfile != NULL) {
                
                if (!incfile->package) {
                    if (curnsname) {
                        curnsname = NULL;
                        indentlevel--;
                        print("};\n");
                    }
                    print("class %s;\n", incfile->name);
                } else {
                    if (curnsname == NULL || strcmp(curnsname, incfile->package)) {
                        if (curnsname) {
                            indentlevel--;
                            print("};\n");
                        }
                        curnsname = incfile->package;
                        print("namespace %s {\n", incfile->package);
                        indentlevel++;
                    }
                    print("class %s;\n", incfile->name);
                }
                incfile = incfile->next;
            }
            if (curnsname) {
                indentlevel--;
                print("}\n\n");
            }
        }

        includes = NULL;
        determine_includes(d, b);
        if (use_corba)
            print("#include <p_orb.h>\n\n");
        if (includes) {
            namelist incfile = includes;
            while (incfile != NULL) {
                if (incfile->package) {
                    if (!strcmp(incfile->package, nsname)) {
                        if (strcmp(incfile->name, name)) {
                            print("#include \"%s.%s\"\n", incfile->name, file_ext);
                        }
                    } else {
                        if (strcmp(incfile->package,"sf") != 0) { // Don't #include from sfml (handled by print_include_stdlib())
                            print("#include \"%s/%s.%s\"\n", incfile->package, incfile->name, file_ext);
                        }
                    }
                } else {
                    print("#include \"%s.%s\"\n", incfile->name, file_ext);
                }
                incfile = incfile->next;
            }
            print("\n");
        }

        print("namespace %s {\n\n", nsname);
        indentlevel++;
        gen_decl(d);
        indentlevel--;
        print("};\n\n");

        indentlevel = 0; /* just for safety (should be 0 already) */
        print("#endif\n");
        fclose(spec);

#ifdef ENABLE_FILE_UPDATE_ON_CHANGE
        update_file_if_changed(b, filename);
#endif

        /////////////////////////////////////
        // Source file
        
        sprintf(filename, "%s/%s.cpp", nsname, name);
        char newfilename[1024];
        sprintf (newfilename, "%s/%s", b->outdir, filename);
        int exists = 0;
        FILE* sourceFile = fopen(newfilename,"r");
        if (sourceFile) {
            exists = 1;
            fclose(sourceFile);
        }
        
        /////////////////////////////////////
        // Source file (generate new)
        if (!exists) {
            /*printf("Create '%s'\n",newfilename);
            spec = open_outfile(filename, b);
            if (spec == NULL) {
                d = d->next;
                continue;
            }
            fclose(spec);*/
        }
        /////////////////////////////////////
        // Source file (update existing)
        else {
            
        }
        
        d = d->next;
    }
}

void
generate_code_cpp (batch *b)
{
    declaration *d;
    umlclasslist tmplist = b->classlist;
    FILE *licensefile = NULL;

    gb = b;

    if (file_ext == NULL)
        file_ext = "h";
    /*
    if (body_file_ext == NULL)
        body_file_ext = "cpp";
     */

    /* open license file */
    if (b->license != NULL) {
        licensefile = fopen (b->license, "r");
        if (!licensefile) {
            fprintf (stderr, "Can't open the license file.\n");
            exit (1);
        }
    }

    while (tmplist != NULL) {
        if (! (is_present (b->classes, tmplist->key->name) ^ b->mask)) {
            push (tmplist, b);
        }
        tmplist = tmplist->next;
    }

    /* Generate a file for each outer declaration.  */
    d = decls;
    while (d != NULL) {
        char *name, *tmpname;
        char filename[BIG_BUFFER];

        if (d->decl_kind == dk_module) {
            name = d->u.this_module->pkg->name;
            if (b->namespaces && !is_present(b->namespaces,name)) {
                d = d->next;
                continue;
            }
#ifdef NO_NAMESPACE_SINGLE_FILE
#if VERBOSE_LEVEL >= 1
            printf("generate_code_cpp(): call gen_namespace for file '%s'\n",name);
#endif
            gen_namespace(b,d);
#endif
        } else {         /* dk_class */
            name = d->u.this_class->key->name;
            /* Do not generate headers for classes without a namespace (unsupported) */
            if (b->namespaces) {
                d = d->next;
                continue;                
            }
#if VERBOSE_LEVEL >= 1
            printf("generate_code_cpp(): file '%s'",name);
#endif
        }
#ifdef ENABLE_FILE_UPDATE_ON_CHANGE
        sprintf (filename, "%s.%s~", name, file_ext);
#else
        sprintf (filename, "%s.%s", name, file_ext);
        printf("Create '%s'\n",filename);
#endif
        
        spec = open_outfile (filename, b);
        if (spec == NULL) {
            d = d->next;
            continue;
        }

        print("// Generated by dia2code\n");
        tmpname = strtoupper(name);
        if (d->decl_kind == dk_module) {
            print("#ifndef __%s__H\n", tmpname);
            print("#define __%s__H\n\n", tmpname);
        }
        else {
            print("#ifndef %s__H\n", tmpname);
            print("#define %s__H\n\n", tmpname);
        }

        /* add license to the header */
        if (b->license != NULL) {
            char lc;
            rewind (licensefile);
            while ((lc = fgetc (licensefile)) != EOF)
                print ("%c", lc);
        }

#ifndef NO_NAMESPACE_SINGLE_FILE
        includes = NULL;
        determine_includes (d, b);
        if (use_corba)
            print ("#include <p_orb.h>\n\n");
        if (includes) {
            namelist incfile = includes;
            while (incfile != NULL) {
                if (!eq (incfile->name, name)) {
                    print ("#include \"%s.%s\"\n", incfile->name, file_ext);
                }
                incfile = incfile->next;
            }
            print ("\n");
        }
#endif

        gen_decl (d);

        indentlevel = 0;  /* just for safety (should be 0 already) */
        print("#endif\n");
        fclose (spec);

#ifdef ENABLE_FILE_UPDATE_ON_CHANGE
        update_file_if_changed(b,filename);
#endif
        
        d = d->next;
    }
}

