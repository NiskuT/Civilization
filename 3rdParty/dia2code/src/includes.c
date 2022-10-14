
#include "includes.h"

namelist includes = NULL;

int have_include (char *name,char* package)
{
    namelist inc = includes;
    while (inc) {
        if (!strcmp (inc->name, name)) {
            if (package && inc->package && !strcmp(inc->package,package))
                return 1;
        }
        inc = inc->next;
    }
    return 0;
}

void add_include (char *name,char* package)
{
    namelist inc = includes;

    if (have_include (name,package))
        return;
    if (inc == NULL) {
        includes = NEW (namenode);
        inc = includes;
    } else {
        while (inc->next)
            inc = inc->next;
        inc->next = NEW (namenode);
        inc = inc->next;
    }
    inc->name = name;
    inc->package = package;
    inc->next = NULL;
}

void push_include (umlclassnode *node)
{
#ifndef NO_NAMESPACE_SINGLE_FILE
    if (node->key->package != NULL) {
        umlpackagelist pkglist = make_package_list (node->key->package);
        add_include (pkglist->key->name);
    } else {
#endif
        umlpackage* package = node->key->package;
        if (!package) {
            add_include (node->key->name,NULL);
        }
        else {
            add_include (node->key->name,package->name);
        }
#ifndef NO_NAMESPACE_SINGLE_FILE
    }
#endif
}

void determine_includes (declaration *d, batch *b)
{
    if (d->decl_kind == dk_module) {
        declaration *inner = d->u.this_module->contents;
        while (inner != NULL) {
            determine_includes (inner, b);
            inner = inner->next;
        }
    } else {
        umlclasslist cl = list_classes (d->u.this_class, b, 0);
        while (cl != NULL) {
            push_include (cl);
            cl = cl->next;
        }
    }
}


void determine_referenced_classes (declaration *d, batch *b)
{
    if (d->decl_kind == dk_module) {
        declaration *inner = d->u.this_module->contents;
        while (inner != NULL) {
            determine_includes (inner, b);
            inner = inner->next;
        }
    } else {
        umlclasslist cl = list_classes (d->u.this_class, b, 1);
        while (cl != NULL) {
            if (!is_enum_stereo(cl->key->stereotype)) {
                push_include (cl);
            }
            cl = cl->next;
        }
    }
}


