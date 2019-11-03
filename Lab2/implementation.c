#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

typedef struct{
	int row;
	int column;
	int colors[3];
}colored_pixel;

colored_pixel colored_pixel_array[1000000];

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveUpReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image left
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveRight(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveRightReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image up
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveDown(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveDownReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param offset - number of pixels to shift the object in bitmap image right
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note1: White pixels RGB(255,255,255) are treated as background. Object in the image refers to non-white pixels.
 * Note2: You can assume the object will never be moved off the screen
 **********************************************************************************************************************/
unsigned char *processMoveLeft(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    return processMoveLeftReference(buffer_frame, width, height, offset);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                               int rotate_iteration) {
    return processRotateCWReference(buffer_frame, width, height, rotate_iteration);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param rotate_iteration - rotate object inside frame buffer counter clockwise by 90 degrees, <iteration> times
 * @return - pointer pointing a buffer storing a modified 24-bit bitmap image
 * Note: You can assume the frame will always be square and you will be rotating the entire image
 **********************************************************************************************************************/
unsigned char *processRotateCCW(unsigned char *buffer_frame, unsigned width, unsigned height,
                                int rotate_iteration) {
    return processRotateCCWReference(buffer_frame, width, height, rotate_iteration);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorX(unsigned char *buffer_frame, unsigned int width, unsigned int height, int _unused) {
    return processMirrorXReference(buffer_frame, width, height, _unused);
}

/***********************************************************************************************************************
 * @param buffer_frame - pointer pointing to a buffer storing the imported 24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param _unused - this field is unused
 * @return
 **********************************************************************************************************************/
unsigned char *processMirrorY(unsigned char *buffer_frame, unsigned width, unsigned height, int _unused) {
    return processMirrorYReference(buffer_frame, width, height, _unused);
}

unsigned char *processTransformations(unsigned char* buffer_frame, colored_pixel *colored_pixel_array, int num_pixels, unsigned width, unsigned height, int translations[4], int rotations, bool mirror_x, bool mirror_y) {
//todo: try to amalgemate all transformations into one function call
    if(translations[0] != 0)
    {
	    for (int pixel = 0; pixel < num_pixels; pixel++) {
	        colored_pixel_array[pixel].row = colored_pixel_array[pixel].row - translations[0];       	
	    }
	}
	else if(translations[2] != 0)
	{
	    for (int pixel = 0; pixel < num_pixels; pixel++) {
	        colored_pixel_array[pixel].row = colored_pixel_array[pixel].row + translations[2];	
        }
  	}

    if(translations[3] != 0)
    {
	    for (int pixel = 0; pixel < num_pixels; pixel++) {
	        colored_pixel_array[pixel].column = colored_pixel_array[pixel].column - translations[3];

        }
  	}
    else if(translations[1] != 0)
    {
	    for (int pixel = 0; pixel < num_pixels; pixel++) {
	        colored_pixel_array[pixel].column = colored_pixel_array[pixel].column + translations[1];
        }
    }

    if(rotations != 0)
    {
    	if(rotations > 0){
		    for (int iteration = 0; iteration < rotations; iteration++) {
			    for (int pixel = 0; pixel < num_pixels; pixel++) {
			    		int render_column = width - 1;
			    		int render_row = 0;
			            int orig_row = colored_pixel_array[pixel].row;
			            int orig_column = colored_pixel_array[pixel].column;
				        colored_pixel_array[pixel].column = render_column - orig_row;
			            colored_pixel_array[pixel].row = render_row + orig_column;
			    }
			}
		}
		else{
			int num_rotations = -(rotations * 3);
			for (int iteration = 0; iteration < num_rotations; iteration++) {
			    for (int pixel = 0; pixel < num_pixels; pixel++) {
			    		int render_column = width - 1;
			    		int render_row = 0;
			            int orig_row = colored_pixel_array[pixel].row;
			            int orig_column = colored_pixel_array[pixel].column;
			            colored_pixel_array[pixel].column = render_column - orig_row;
			            colored_pixel_array[pixel].row = render_row + orig_column;
			    }
			}
		}
 	}


	if(mirror_x){
		for (int pixel = 0; pixel < num_pixels; pixel++) {
	        colored_pixel_array[pixel].row = (height - colored_pixel_array[pixel].row - 1);
	    }
    }

	if(mirror_y){
		for (int pixel = 0; pixel < num_pixels; pixel++) {
	    	colored_pixel_array[pixel].column = (width - colored_pixel_array[pixel].column - 1);
	    }
	}


    for(int pixel = 0; pixel < num_pixels; pixel++){
    	int pixel_position = (colored_pixel_array[pixel].row * width + colored_pixel_array[pixel].column) * 3;
    	buffer_frame[pixel_position] = colored_pixel_array[pixel].colors[0];
    	buffer_frame[pixel_position + 1] = colored_pixel_array[pixel].colors[1];
    	buffer_frame[pixel_position + 2] = colored_pixel_array[pixel].colors[2];

    }

    return buffer_frame;
}
/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "if-cracked";

    // Please fill in your information
    char student_first_name[] = "Murton";
    char student_last_name[] = "Liu";
    char student_student_number[] = "1002734231";

    // Printing out team information
    printf("*******************************************************************************************************\n");
    printf("Team Information:\n");
    printf("\tteam_name: %s\n", team_name);
    printf("\tstudent_first_name: %s\n", student_first_name);
    printf("\tstudent_last_name: %s\n", student_last_name);
    printf("\tstudent_student_number: %s\n", student_student_number);
}

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          You can modify anything else in this file
 ***********************************************************************************************************************
 * @param sensor_values - structure stores parsed key value pairs of program instructions
 * @param sensor_values_count - number of valid sensor values parsed from sensor log file or commandline console
 * @param frame_buffer - pointer pointing to a buffer storing the imported  24-bit bitmap image
 * @param width - width of the imported 24-bit bitmap image
 * @param height - height of the imported 24-bit bitmap image
 * @param grading_mode - turns off verification and turn on instrumentation
 ***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void implementation_driver(struct kv *sensor_values, int sensor_values_count, unsigned char *frame_buffer,
                           unsigned int width, unsigned int height, bool grading_mode) {
    int directional_transforms[4] = {0};
    int rotational_transform = 0;
    bool mirror_x = 0;
    bool mirror_y = 0;
    int current_value = 0;
    int num_colored_pixels = 0;
    int location = 0;

    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            if(frame_buffer[location] != 255 || frame_buffer[location + 1] != 255 || frame_buffer[location + 2] != 255){
	            // printf("[%03d,%03d,%03d] at location %d is not white\n", frame_buffer[location], frame_buffer[location + 1], frame_buffer[location + 2], location);
	            colored_pixel_array[num_colored_pixels].row = row;
	            colored_pixel_array[num_colored_pixels].column = column;
	            colored_pixel_array[num_colored_pixels].colors[0] = frame_buffer[location];
	            colored_pixel_array[num_colored_pixels].colors[1] = frame_buffer[location + 1];
	            colored_pixel_array[num_colored_pixels].colors[2] = frame_buffer[location + 2];
	            num_colored_pixels++;
            }
            location += 3;
        }
    }

    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
        // printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
        //        sensor_values[sensorValueIdx].value);
        current_value = sensor_values[sensorValueIdx].value;
  //       int transformation = convertToInt(current_value.key);
        switch(sensor_values[sensorValueIdx].key[0]) 
        {
        	// printf("%c\n", *current_value.key);
	        case('W'):
	        	if(!mirror_x){
					directional_transforms[(4 - rotational_transform) & 3] += current_value;
				}
				else{
					directional_transforms[(6 - rotational_transform) & 3] += current_value;	
				}
				break;
			case('D'):
	        	if(!mirror_y){
					directional_transforms[(5 - rotational_transform) & 3] += current_value;
				}
				else{
					directional_transforms[(7 - rotational_transform) & 3] += current_value;	
				}				
				break;
			case('S'):
	        	if(!mirror_x){
					directional_transforms[(6 - rotational_transform) & 3] += current_value;
				}
				else{
					directional_transforms[(4 - rotational_transform) & 3] += current_value;	
				}				
				break;
			case('A'):
	        	if(!mirror_y){
					directional_transforms[(7 - rotational_transform) & 3] += current_value;
				}
				else{
					directional_transforms[(5 - rotational_transform) & 3] += current_value;	
				}				
				break;
			case('C'):
				if(mirror_x == 1 && mirror_y == 1){
					current_value *= -1;
					mirror_x = 0;
					mirror_y = 0;
				}
				else if(mirror_x || mirror_y){
					current_value *= -1;
				}
				if(sensor_values[sensorValueIdx].key[1] != 'W'){
					current_value *= -1;
				}
				rotational_transform += current_value;
				break;
			case('M'):
				if(sensor_values[sensorValueIdx].key[1] == 'X'){
					mirror_x = !mirror_x;
					break;
				}
				else{
					mirror_y = !mirror_y;
					break;
				}
			default:
				break;
		}

		// printf("Current translations are for key #%d %s with value %d are: %d %d %d %d %d %d %d\n", sensorValueIdx + 1, sensor_values[sensorValueIdx].key, current_value,
		// directional_transforms[0], directional_transforms[1], directional_transforms[2], directional_transforms[3], rotational_transform, mirror_x, mirror_y);

		rotational_transform %= 4;

		if((sensorValueIdx + 1) % 25 == 0)
		{
		    for(int pixel = 0; pixel < num_colored_pixels; pixel++){
		            int position_buffer_frame = (colored_pixel_array[pixel].row * width + colored_pixel_array[pixel].column) * 3;
		            frame_buffer[position_buffer_frame] = 255;
		            frame_buffer[position_buffer_frame + 1] = 255;
		            frame_buffer[position_buffer_frame + 2] = 255;
		    }

			if(directional_transforms[0] > directional_transforms[2]){
				directional_transforms[0] -= directional_transforms[2];
				directional_transforms[2] = 0;
			}
			else if(directional_transforms[2] > directional_transforms[0]){
				directional_transforms[2] -= directional_transforms[0];
				directional_transforms[0] = 0;
			}
			else{
				directional_transforms[0] = 0;
				directional_transforms[2] = 0;
			}

			if(directional_transforms[1] > directional_transforms[3]){
				directional_transforms[1] -= directional_transforms[3];
				directional_transforms[3] = 0;
			}
			else if(directional_transforms[3] > directional_transforms[1]){
				directional_transforms[3] -= directional_transforms[1];
				directional_transforms[1] = 0;
			}
			else{
				directional_transforms[1] = 0;
				directional_transforms[3] = 0;
			}

			// if(directional_transforms[0] < 0){
			// 	directional_transforms[2] *= -1 * directional_transforms[0];
			// }
			// if(directional_transforms[1] < 0){
			// 	directional_transforms[3] *= -1 * directional_transforms[1];
			// }
			// if(directional_transforms[2] < 0){
			// 	directional_transforms[0] *= -1 * directional_transforms[2];
			// }
			// if(directional_transforms[3] < 0){
			// 	directional_transforms[1] *= -1 * directional_transforms[3];
			// }
			// printf("Translations are %d %d %d %d %d %d %d\n", directional_transforms[0], directional_transforms[1], directional_transforms[2], directional_transforms[3]
		 //     	,rotational_transform, mirror_x, mirror_y);

		    frame_buffer = processTransformations(frame_buffer, colored_pixel_array, num_colored_pixels, width, height, directional_transforms, rotational_transform, mirror_x, mirror_y);

		    //printBMP(width,height,frame_buffer);
		    verifyFrame(frame_buffer, width, height, grading_mode);
		    for(int i = 0; i < 4; i++){
		    	directional_transforms[i] = 0;
		    }
		    rotational_transform = 0;
		    mirror_x = 0;
		    mirror_y = 0;
		    // if(sensorValueIdx % 50 == 0){
		    // 	printBMP(width, height, frame_buffer);
		    //}
		}
    }
    return;
}
