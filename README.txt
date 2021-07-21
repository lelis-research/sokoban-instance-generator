******* ******* Instructions on how to run the system, one instance at a time ******* ******* 

1) Create a folder 'Results' in the main folder of the project, in case the folder isn't already there. 

2) - Set the following paths in the Makefile according to the path in your machine:
	BLOSSOM_DIR=/home/damaris/git/instance_generation/Local_Search/Include/blossom5/
	MEMORY_DIR=/home/damaris/git/instance_generation/Local_Search/Include/memory/


2) Type 'make LocalSearch' to compile the code

3) Run the code by running: ./bin/LocalSearch ${NAME} ${HEUR} ${TIME} ${RUN}

- NAME: Name of the instance
	- Do not add the extension of the instance (ex: instance_01) 
	- The instance file must be pre-processed by placing the boxes into the goal position, as shown in the example below, where # denotes a wall, @ the man, and * a box on a goal location:
			    #####
			    #   #
			    #   #
			  ###   ##
			  #      #
			### # ## #   ######
			#   # ## #####  **#
			#               **#
			##### ### #@##  **#
			    #     #########
			    #######
	
- HEUR: Name of the heuristic function used. The available options are as follows: pdb1, pdb2, pdb3r, pdb4r, pdb2_2oc, pdb3r_3ocr, pdb4r_4ocr, 2oc_pdb2, 3ocr_pdb3r, 4ocr_pdb4r, wpdb1_pdb1, wpdb2_pdb2, wpdb3r_pdb3r, wpdb4r_pdb4r, wpdb2_pdb2_2oc, wpdb3r_pdb3r_3ocr, wpdb4r_pdb4r_4ocr, wpbd2_2oc_pdb2, wpbd3r_3ocr_pdb3r, wpdb4r_4ocr_pdb4r
(see https://www.ijcai.org/Proceedings/2019/0646.pdf for details)

- TIME: Allowed running time in seconds

- RUN: Number of times the algorithm will be executed. In our experiments we run the algorithm several times and return the instance with largest objective value. 

