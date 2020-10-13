#include <stdio.h>
#include <string.h>
#include <stdbool.h>
# include <stdlib.h>
#include "utilities.h"  // DO NOT REMOVE this line
#include "implementation_reference.h"   // DO NOT REMOVE this line

int separateRGB(unsigned char * frame_buffer, int *rows, int *cols, unsigned char *red, unsigned char *green,
                unsigned char *blue, int image_size, int row_size){
    unsigned char r, g, b;
    int count = 0;
    for(int i = 0; i < image_size; i+=3){
        r = frame_buffer[i];
        g = frame_buffer[i+1];
        b  = frame_buffer[i+2];
        if(r != 255 || g != 255 || b != 255){
            /* If not blank, record colored pixels */
            red[count] = r;
            green[count] = g;
            blue[count] = b;
            rows[count] = i/row_size;
            cols[count] = (i%row_size) / 3;
            count++;
        }
    }
    //printf("in separate RGB");
    return count;
}

void whiteImage(unsigned char * frame_buffer, int num_colored_pixels, int row_size,
                int * rows, int * cols){
    for(int i=0; i<num_colored_pixels; i++){
        frame_buffer[rows[i]*row_size + cols[i] *3] = 255;
        frame_buffer[rows[i]*row_size + cols[i] *3 +1] = 255;
        frame_buffer[rows[i]*row_size + cols[i] *3 +2] = 255;
    }
    //printf("in whiteImage");
}

void fillFrameBuffer(unsigned char *frame_buffer, int num_colored_pixels, int row_size,
                     int *rows, int *cols, unsigned char *red, unsigned char *green, 
                     unsigned char * blue){
    for(int i=0; i<num_colored_pixels; i++){
        frame_buffer[rows[i]*row_size + cols[i] *3] = red[i];
        frame_buffer[rows[i]*row_size + cols[i] *3 + 1] = green[i];
        frame_buffer[rows[i]*row_size + cols[i] *3 + 2] = blue[i];
    }
    //printf("in fillFrameBuffer");
}

void upDownLeftRight(int upCount, int rightCount, int num_colored_pixels, int *rows, int *cols){
    for(int i=0; i<num_colored_pixels; i++){
        rows[i] -= upCount;
        cols[i] += rightCount;
    }
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
unsigned char *processMoveUp(unsigned char *buffer_frame, unsigned width, unsigned height, int offset) {
    // handle negative offsets
    if (offset < 0){
        return processMoveDownReference(buffer_frame, width, height, offset * -1);
    }

    // allocate memory for temporary image buffer
    unsigned char *rendered_frame = allocateFrame(width, height);

    // store shifted pixels to temporary buffer
    for (int row = 0; row < (height - offset); row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            int position_buffer_frame = (row + offset) * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = buffer_frame[position_buffer_frame];
            rendered_frame[position_rendered_frame + 1] = buffer_frame[position_buffer_frame + 1];
            rendered_frame[position_rendered_frame + 2] = buffer_frame[position_buffer_frame + 2];
        }
    }

    // fill left over pixels with white pixels
    for (int row = (height - offset); row < height; row++) {
        for (int column = 0; column < width; column++) {
            int position_rendered_frame = row * width * 3 + column * 3;
            rendered_frame[position_rendered_frame] = 255;
            rendered_frame[position_rendered_frame + 1] = 255;
            rendered_frame[position_rendered_frame + 2] = 255;
        }
    }

    // copy the temporary buffer back to original frame buffer
    buffer_frame = copyFrame(rendered_frame, buffer_frame, width, height);

    // free temporary image buffer
    deallocateFrame(rendered_frame);

    // return a pointer to the updated image buffer
    return buffer_frame;}

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

/***********************************************************************************************************************
 * WARNING: Do not modify the implementation_driver and team info prototype (name, parameter, return value) !!!
 *          Do not forget to modify the team_name and team member information !!!
 **********************************************************************************************************************/
void print_team_info(){
    // Please modify this field with something interesting
    char team_name[] = "I want a speeding ticket";

    // Please fill in your information
    char student_first_name[] = "Chinmayee";
    char student_last_name[] = "Gidwani";
    char student_student_number[] = "1003062473";

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
    int processed_frames = 0;
    int upCount = 0;
    int rightCount = 0;
    bool mirrorX = false;
    bool mirrorY = false;
    int image_size = width * height;
    int row_size = 3*width;
    int image_size_bytes = image_size * 3;
    unsigned char *red = (unsigned char *) malloc(image_size);
    unsigned char *green = (unsigned char *) malloc(image_size);
    unsigned char *blue = (unsigned char *) malloc(image_size);
    int * rows = (int *) malloc(image_size * sizeof(int));
    int * cols = (int *) malloc(image_size * sizeof(int));
    int num_colored_pixels = separateRGB(frame_buffer, rows, cols, red, green, blue, image_size, row_size);
    whiteImage(frame_buffer, num_colored_pixels, row_size, rows, cols);
    //fillFrameBuffer(frame_buffer, num_colored_pixels, row_size, rows, cols,
    //                red, green, blue);
    //can allocate frame here later
    for (int sensorValueIdx = 0; sensorValueIdx < sensor_values_count; sensorValueIdx++) {
//        printf("Processing sensor value #%d: %s, %d\n", sensorValueIdx, sensor_values[sensorValueIdx].key,
//               sensor_values[sensorValueIdx].value);
        if (!strcmp(sensor_values[sensorValueIdx].key, "W")) { //right
            upCount += sensor_values[sensorValueIdx].value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "A")) { //left
            rightCount -= sensor_values[sensorValueIdx].value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "S")) { //down
            upCount -= sensor_values[sensorValueIdx].value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "D")) {//up
            rightCount += sensor_values[sensorValueIdx].value;
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CW")) {
            if(upCount!=0 || rightCount != 0){
                printf("upCount: %d \n", upCount);
                printf("rightCount: %d \n", rightCount);
                upDownLeftRight(upCount, rightCount, num_colored_pixels, rows, cols);
                upCount=0;
                rightCount=0; //reset counters
            }
            frame_buffer = processRotateCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "CCW")) {

            if(upCount!=0 || rightCount != 0){
                printf("upCount: %d \n", upCount);
                printf("rightCount: %d \n", rightCount);
                upDownLeftRight(upCount, rightCount, num_colored_pixels, rows, cols);
                upCount=0;
                rightCount=0; //reset counters
            }
            frame_buffer = processRotateCCW(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MX")) {
            if(upCount!=0 || rightCount != 0){
                printf("upCount: %d \n", upCount);
                printf("rightCount: %d \n", rightCount);
                upDownLeftRight(upCount, rightCount, num_colored_pixels, rows, cols);
                upCount=0;
                rightCount=0; //reset counters
            }
            frame_buffer = processMirrorX(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
        } else if (!strcmp(sensor_values[sensorValueIdx].key, "MY")) {
            if(upCount!=0 || rightCount != 0){
                printf("upCount: %d \n", upCount);
                printf("rightCount: %d \n", rightCount);
                upDownLeftRight(upCount, rightCount, num_colored_pixels, rows, cols);
                upCount=0;
                rightCount=0; //reset counters
            }
            frame_buffer = processMirrorY(frame_buffer, width, height, sensor_values[sensorValueIdx].value);
        }
        processed_frames += 1;
        if (processed_frames % 25 == 0) {
            if(upCount!=0 || rightCount != 0){
                printf("upCount: %d \n", upCount);
                printf("rightCount: %d ", rightCount);
                upDownLeftRight(upCount, rightCount, num_colored_pixels, rows, cols);
                upCount=0;
                rightCount=0; //reset counters
            }
            fillFrameBuffer(frame_buffer, num_colored_pixels, row_size, rows, cols,
                       red, green, blue);
            verifyFrame(frame_buffer, width, height, grading_mode);
            whiteImage(frame_buffer, num_colored_pixels, row_size, rows, cols);
        }
    }
    free(rows);
    free(cols);
    free(red);
    free(green);
    free(blue);
    return;
}
