# Implemented continuous integration

We chose to use continuous integration to automate many tasks in our project. The continuous iteration tool we use is GitHub actions. If one of these actions fail during a pull request, the merge request cannot be authorized.

## Build the documentation

We have chosen to generate the documentation of our project with doxygen. In order to automate the deployment of the website created thanks to doxygen, we used a github action which automatically builds the site then a second one which deploys it.

The deployment of the site is done directly from the GitHub branch ['gh-pages'](<https://github.com/NiskuT/Civilization/tree/gh-pages>) thanks to the GitHub pages feature.

## Build the project

We also use actions to build our project in different environments and thus check the compatibility of our project with differents configurations and OS (linux, windows).

## Run tests

GitHub also does coverage testing to make sure our project is working properly.

## Valgrind analysis for memory leaks

We chose to use valgrind. It is a dynamic analysis tool which allows to analyze a program and to detect possible memory leaks.
