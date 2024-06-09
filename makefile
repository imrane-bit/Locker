testingstructs :
	gcc headers/eror_handling/eror_handling.c	utilities/initialize/initialize.c utilities/structutils/struct-utils.c cpassmann.c -I utilities/initialize -I utilities/structutils -I headers/eror_handling -o testinginitialize

optimization_testing :
	gcc -O3 headers/eror_handling/eror_handling.c	utilities/initialize/initialize.c utilities/structutils/struct-utils.c cpassmann.c -I utilities/initialize -I utilities/structutils -I headers/eror_handling -o testinginitialize

debug :
	gcc -g headers/eror_handling/eror_handling.c	utilities/initialize/initialize.c utilities/structutils/struct-utils.c cpassmann.c -I utilities/initialize -I utilities/structutils -I headers/eror_handling -o testinginitialize


