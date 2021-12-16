//
//  parser.cpp
//
//  Editor: Danial Khan
//  Date: December 16th


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

//*********************This is the shape array, to be dynamically allocated*********************//
Shape** shapesArray;

//*********************The number of shapes in the database, to be incremented*********************//
int shapeCount = 0;

//*********************The value of the argument to the maxShapes command*********************//
int max_shapes;

//*********************Helper functions*********************

//Checking for the lack of inputted arguments
bool check_few_args(stringstream & lineStream);
//Special case for lack of arguments due to whiteSpaces
bool check_few_args_whiteSpace(stringstream & lineStream, bool few_args);
//Checking if user inputted an invalid value
bool invalid_value_helper(stringstream & lineStream, int value, bool &invalid_argument);
//Checking for invalid arguments inputted
bool check_arg_helper(stringstream & lineStream, bool &invalid_shape_type);
//Checking for too many arguments
bool too_many_helper(stringstream & lineStream, bool invalid_arg);
//Checking if input is not a string when required
void error_check_string(stringstream &lineStream, string &string1, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument);
//Checking if integers inputted lineStream
void error_check_loc_size(stringstream &lineStream, int &int_value, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument, bool &invalid_value, bool &invalid_shape_type);
void error_check_rotate(stringstream &lineStream, int &angle, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument, bool &invalid_value, bool &angle_passed, bool &invalid_shape_type);
//Checking to ensure that the name is valid
void error_check_name(stringstream &lineStream, string &name, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument);
void peek_checker(stringstream &lineStream, bool &invalid_argument);

int main() {

    string line;
    string command;

    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while (!cin.eof()) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream(line);

        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here
        string command_array[] = {"maxShapes", "create", "move", "rotate", "draw", "delete"};
        bool invalid_command{false};

        for (int i{0}; i < 6; i++) {
            if (command == command_array[i]) {
                break;
            }
            if (i == 5) {
                invalid_command = true;
            }
        }

        if (invalid_command) {
            cout << "Error: invalid command" << endl;
        } else {
            //boolean variables used to frame program design
            bool invalid_argument{false};
            bool invalid_shape_name{false};
            bool shape_name_exists{false};
            bool shape_name_not_found{false};
            bool invalid_shape_type{true};
            bool invalid_value{false};
            bool too_many_args{false};
            bool too_few_args{false};
            bool too_few_args_whitespace{false};
            bool shape_array_full{false};

            if (command == "maxShapes") {

                int value{0};

                //check too less arguments
                too_few_args = check_few_args(lineStream);


                lineStream >> value;


                //too few arguments corner case, since the user could enter a whitespace
                too_few_args_whitespace = check_few_args_whiteSpace(lineStream, too_few_args);


                //check for invalid argument
                invalid_argument = check_arg_helper(lineStream, invalid_shape_type);

                //make sure that code doesn't skip any value other than ' ' or eof
                peek_checker(lineStream, invalid_argument);


                //check for invalid value:
                invalid_value = invalid_value_helper(lineStream, value, invalid_argument);

                // check too many args
                too_many_args = too_many_helper(lineStream, invalid_argument);

                lineStream.clear();
                lineStream.ignore(1000, 'n');

                if (!invalid_argument && !invalid_value && !too_many_args && !too_few_args &&
                    !too_few_args_whitespace) {
                    if (shapesArray != NULL) {
                        for (int i{0}; i < max_shapes; i++) {
                            delete shapesArray[i];
                            shapesArray[i] = NULL;
                            shapeCount = 0;
                        }
                    }
                    cout << "New database: max shapes is " << value << endl;
                    max_shapes = value;
                    shapesArray = new Shape *[max_shapes];
                }
            }
            if (command == "create") {
                bool name_passed{false};
                //arguments include name, type, loc, loc, size, size
                string name, type;
                int integer_test;
                int loc1, loc2, size1, size2;
                string restrictedTypes[11] = {"all", "maxShapes", "create", "move", "rotate", "draw", "delete",
                                              "circle", "ellipse", "rectangle", "triangle"};

                //******CHECKING THE NAME**********

                error_check_name(lineStream, name, too_few_args, too_few_args_whitespace, invalid_argument);

                //invalid shape name:

                for (int i{0}; i < 11; i++) {
                    if (name == restrictedTypes[i]) {
                        cout << "Error: invalid shape name" << endl;
                        invalid_shape_name = true;
                    }
                }

                //shape name exists:
                for (int i{0}; i < max_shapes; i++) {
                    if (shapesArray[i] != NULL) {
                        if (name == shapesArray[i]->getName()) {
                            cout << "Error: shape name exists" << endl;
                            shape_name_exists = true;
                        }
                    }
                }

                if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !shape_name_exists &&
                    !invalid_shape_name) {
                    name_passed = true;
                }

                if (name_passed) {
                    //*******CHECKING THE TYPE*******
                    bool type_passed{false};
                    error_check_string(lineStream, type, too_few_args, too_few_args_whitespace, invalid_argument);
                    //invalid shape type:
                    for (int i{0}; i < 4; i++) {
                        if (type == shapeTypesList[i]) {
                            invalid_shape_type = false;
                            break;
                        }
                    }

                    //if (invalid_shape_type && !invalid_argument && !too_few_args && !too_few_args_whitespace) {
                    if (invalid_shape_type && !too_few_args && !too_few_args_whitespace) {

                        cout << "Error: invalid shape type" << endl;
                    }
                   // }

                    if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !invalid_shape_type) {
                        type_passed = true;
                    }


                    if (type_passed) {
                        //*****loc1 ********
                        bool loc1_passed{false};
                        error_check_loc_size(lineStream, loc1, too_few_args, too_few_args_whitespace, invalid_argument,
                                             invalid_value, invalid_shape_type);

                        if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !invalid_value) {
                            loc1_passed = true;
                        }

                        if (loc1_passed) {
                            //*****loc2*****
                            bool loc2_passed{false};
                            error_check_loc_size(lineStream, loc2, too_few_args, too_few_args_whitespace,
                                                 invalid_argument,
                                                 invalid_value,invalid_shape_type);

                            if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !invalid_value) {
                                loc2_passed = true;
                            }

                            if (loc2_passed) {
                                //*****size1*****
                                bool size1_passed{false};
                                error_check_loc_size(lineStream, size1, too_few_args, too_few_args_whitespace,
                                                     invalid_argument,
                                                     invalid_value, invalid_shape_type);

                                if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !invalid_value) {
                                    size1_passed = true;
                                }

                                if (size1_passed) {
                                    //*****size2*****
                                    bool size2_passed{false};
                                    error_check_loc_size(lineStream, size2, too_few_args, too_few_args_whitespace,
                                                         invalid_argument,
                                                         invalid_value, invalid_shape_type);

                                    if (!too_few_args && !too_few_args_whitespace && !invalid_argument &&
                                        !invalid_value) {
                                        size2_passed = true;
                                    }

                                    if((size1 != size2) && type == "circle"){
                                        size2_passed = false;
                                        cout << "Error: invalid value" << endl;
                                    }

                                    if (size2_passed) {
                                        //too many arguments:
                                        too_many_args = too_many_helper(lineStream, invalid_argument);


                                        if (!too_many_args) {
                                            //shape array is full:
                                            bool array_full{false};

                                            if (shapeCount == max_shapes) {
                                                cout << "Error: Shape array is full" << endl;
                                                array_full = true;
                                            }

                                            if (!array_full) {
                                                //Create the actual argument:
                                                shapeCount += 1;
                                                shapesArray[shapeCount - 1] = new Shape(name, type, loc1, size1, loc2,
                                                                                        size2);

                                                cout << "Created " << name << ": " << type << " "
                                                     << loc1 << " " << loc2 << " "
                                                     << size1 << " " << size2 << endl;

                                                lineStream.clear();
                                                lineStream.ignore(1000, 'n');

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (command == "move") {
                //arguments include name, loc, loc
                string name;
                int loc1, loc2, shape_index;

                //*******NAME*******
                bool name_passed{false};
                error_check_name(lineStream, name, too_few_args, too_few_args_whitespace, invalid_argument);

                //shape name not found?:
                for (int i{0}; i < max_shapes; i++) {
                    if (shapesArray[i] != NULL) {
                        if (name == shapesArray[i]->getName()) {
                            shape_name_exists = true;
                            shape_index = i;
                        }
                    }
                }
                if (!shape_name_exists && !invalid_argument && !too_few_args && !too_few_args_whitespace) {
                    cout << "Error: shape " << name << " not found" << endl;
                }

                if (!too_few_args && !too_few_args_whitespace && !invalid_argument && shape_name_exists &&
                    !invalid_shape_name) {
                    name_passed = true;
                }

                if (name_passed) {
                    //****** CHECK THE LOC1 ********
                    bool loc1_passed{false};
                    error_check_loc_size(lineStream, loc1, too_few_args, too_few_args_whitespace, invalid_argument,
                                         invalid_value, invalid_shape_type);

                    if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !invalid_value) {
                        loc1_passed = true;
                    }

                    if (loc1_passed) {
                        //*****loc2*****
                        bool loc2_passed{false};
                        error_check_loc_size(lineStream, loc2, too_few_args, too_few_args_whitespace, invalid_argument,
                                             invalid_value, invalid_shape_type);

                        if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !invalid_value) {
                            loc2_passed = true;
                        }
                        if (loc2_passed) {
                            too_many_args = too_many_helper(lineStream, invalid_argument);
                            if (!too_many_args) {
                                shapesArray[shape_index]->setXlocation(loc1);
                                shapesArray[shape_index]->setYlocation(loc2);
                                cout << "Moved " << name << " to " << loc1 << " " << loc2 << endl;

                                lineStream.clear();
                                lineStream.ignore(1000, 'n');
                            }
                        }
                    }
                }
            }
            if (command == "rotate") {

                //arguments include name, angle
                string name;
                int angle, shape_index;
                //******NAME******
                bool name_passed{false};
                error_check_name(lineStream, name, too_few_args, too_few_args_whitespace, invalid_argument);

                //shape name not found?:
                for (int i{0}; i < max_shapes; i++) {
                    if (shapesArray[i] != NULL) {
                        if (name == shapesArray[i]->getName()) {
                            shape_name_exists = true;
                            shape_index = i;
                        }
                    }
                }
                if (!shape_name_exists && !invalid_argument) {
                    cout << "Error: shape " << name << " not found" << endl;
                }

                if (!too_few_args && !too_few_args_whitespace && !invalid_argument && shape_name_exists &&
                    !invalid_shape_name) {
                    name_passed = true;
                }

                if (name_passed) {
                    bool angle_passed{false};
                    error_check_rotate(lineStream, angle, too_few_args, too_few_args_whitespace, invalid_argument,
                                         invalid_value, angle_passed, invalid_shape_type);

                    if (!too_few_args && !too_few_args_whitespace && !invalid_argument && !invalid_value) {
                        angle_passed = true;
                    }

                    if (angle_passed) {

                        too_many_args = too_many_helper(lineStream, invalid_argument);
                        if (!too_many_args) {
                            shapesArray[shape_index]->setRotate(angle);
                            cout << "Rotated " << name << " by " << angle << " degrees" << endl;

                            lineStream.clear();
                            lineStream.ignore(1000, 'n');
                        }
                    }
                }
            }
            if (command == "draw") {

                //two options for the draw command:
                //CASE #1: name (only draws one shape)
                string name;
                int shape_index;
                bool name_passed{false};
                error_check_name(lineStream, name, too_few_args, too_few_args_whitespace, invalid_argument);

                if (name == "all") {
                    too_many_args = too_many_helper(lineStream, invalid_argument);
                    if (!too_many_args) {

                        cout << "Drew all shapes" << endl;
                        for (int i{0}; i < max_shapes; i++) {
                            if (shapesArray[i] != NULL) {
                                shapesArray[i]->draw();
                            }
                        }
                        lineStream.clear();
                        lineStream.ignore(1000, 'n');
                    }
                } else {
                    //shape name not found?:
                    for (int i{0}; i < max_shapes; i++) {
                        if (shapesArray[i] != NULL) {
                            if (name == shapesArray[i]->getName()) {
                                shape_name_exists = true;
                                shape_index = i;
                                break;
                            }
                        }
                    }
                    if (!shape_name_exists && !invalid_argument && !too_few_args && !too_few_args_whitespace) {
                        cout << "Error: shape " << name << " not found" << endl;
                    }

                    if (!too_few_args && !too_few_args_whitespace && !invalid_argument && shape_name_exists &&
                        !invalid_shape_name) {
                        name_passed = true;
                    }

                    if (name_passed) {
                        too_many_args = too_many_helper(lineStream, invalid_argument);
                        if (!too_many_args) {

                            cout << "Drew ";
                            shapesArray[shape_index]->draw();

                            lineStream.clear();
                            lineStream.ignore(1000, 'n');
                        }
                    }
                }
            }
            if (command == "delete") {

                //two options for the delete command:
                //CASE #1: name (only draws one shape)
                string name;
                int shape_index;
                bool name_passed{false};
                error_check_name(lineStream, name, too_few_args, too_few_args_whitespace, invalid_argument);

                if (name == "all") {
                    too_many_args = too_many_helper(lineStream, invalid_argument);
                    if (!too_many_args) {

                        cout << "Deleted: all shapes" << endl;
                        for (int i{0}; i < max_shapes; i++) {
                            if (shapesArray != NULL) {
                                delete shapesArray[i];
                                shapesArray[i] = NULL;
                                shapeCount -= 1;
                            }
                        }
                        lineStream.clear();
                        lineStream.ignore(1000, 'n');
                    }
                } else {
                    //shape name not found?:
                    for (int i{0}; i < max_shapes; i++) {
                        if (shapesArray[i] != NULL) {
                            if (name == shapesArray[i]->getName()) {
                                shape_name_exists = true;
                                shape_index = i;
                            }
                        }
                    }
                    if (!shape_name_exists && !invalid_argument && !too_few_args && !too_few_args_whitespace) {
                        cout << "Error: shape " << name << " not found" << endl;
                    }

                    if (!too_few_args && !too_few_args_whitespace && !invalid_argument && shape_name_exists &&
                        !invalid_shape_name) {
                        name_passed = true;
                    }

                    if (name_passed) {
                        too_many_args = too_many_helper(lineStream, invalid_argument);
                        if (!too_many_args) {

                            cout << "Deleted shape " << shapesArray[shape_index]->getName() << endl;
                            delete shapesArray[shape_index];
                            shapesArray[shape_index] = NULL;
                            //shapeCount -= 1;

                            lineStream.clear();
                            lineStream.ignore(1000, 'n');
                        }
                    }
                }
            }
        }
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);


        // End input loop until EOF.
    }

    return 0;
}

bool check_few_args(stringstream & lineStream){
    if(lineStream.eof()){
        cout << "Error: too few arguments" << endl;
        return true;
    }
    return false;
}

bool check_few_args_whiteSpace(stringstream & lineStream, bool few_args){
    if(lineStream.eof() && lineStream.fail() && !few_args){
        cout << "Error: too few arguments" << endl;
        return true;
    }
    return false;
}

bool invalid_value_helper(stringstream & lineStream, int value, bool &invalid_argument){
    if(value < 0 && !invalid_argument){
        cout << "Error: invalid value" << endl;
        return true;
    }
    return false;
};

bool check_arg_helper(stringstream & lineStream, bool &invalid_shape_type){
    //int integer_test;
    //lineStream >> integer_test; // checking if they entered an integer

    if(lineStream.fail() && !lineStream.eof() && !invalid_shape_type){
        cout << "Error: invalid argument" << endl;
        return true;
    }
    return false;
}

bool too_many_helper(stringstream & lineStream, bool invalid_arg){
    lineStream >> ws;

    if(!lineStream.eof() && !invalid_arg){
        cout << "Error: too many arguments" << endl;
        return true;
    }
    return false;
}

void error_check_loc_size(stringstream &lineStream, int &int_value, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument, bool &invalid_value, bool &invalid_shape_type){
    //check too less arguments
    too_few_args = check_few_args(lineStream);


    lineStream >> int_value;

    //too few arguments corner case, since the user could enter a whitespace
    too_few_args_whitespace = check_few_args_whiteSpace(lineStream, too_few_args);


    //check for invalid argument
    invalid_argument = check_arg_helper(lineStream, invalid_shape_type);

    //make sure that code doesn't skip any value other than ' ' or eof
    peek_checker(lineStream, invalid_argument);


    //check for invalid value:
    invalid_value = invalid_value_helper(lineStream, int_value, invalid_argument);


}

void error_check_rotate(stringstream &lineStream, int &angle, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument, bool &invalid_value, bool &angle_passed, bool &invalid_shape_type){
    //check too less arguments
    too_few_args = check_few_args(lineStream);


    lineStream >> angle;

    //too few arguments corner case, since the user could enter a whitespace
    too_few_args_whitespace = check_few_args_whiteSpace(lineStream, too_few_args);


    //check for invalid argument
    invalid_argument = check_arg_helper(lineStream, invalid_shape_type);

    //make sure that code doesn't skip any value other than ' ' or eof
    peek_checker(lineStream, invalid_argument);


    //check for invalid value:
    if(angle > 360 || angle < 0){
        angle_passed = false;
        cout << "Error: invalid value" << endl;
        invalid_value = true;
    }

}


void error_check_string(stringstream &lineStream, string &string1, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument){
    int integer_test;
    too_few_args = check_few_args(lineStream);

    lineStream >> integer_test; // checking if they entered an integer
    too_few_args_whitespace = check_few_args_whiteSpace(lineStream, too_few_args);

    if(lineStream.fail() && !lineStream.eof() && !too_few_args){
        lineStream.clear();
        lineStream >> string1;
    }
    else{
        if(!too_few_args && !too_few_args_whitespace){
            invalid_argument = true;
            cout << "Error: invalid argument" << endl;

            //peek_checker(lineStream, invalid_argument);
        }
    }


    //invalid argument:
    //lineStream >> string1; // checking if they entered an integer

    //invalid_argument = check_arg_helper(lineStream);

}

void error_check_name(stringstream &lineStream, string &name, bool &too_few_args, bool &too_few_args_whitespace, bool &invalid_argument){
    //check too less arguments
    int integer_test;
    too_few_args = check_few_args(lineStream);


    //invalid argument:
    lineStream >> integer_test; // checking if they entered an integer
    too_few_args_whitespace = check_few_args_whiteSpace(lineStream, too_few_args);

    if(lineStream.fail() && !lineStream.eof() && !too_few_args){
        lineStream.clear();
        lineStream >> name;
    }
    else{
        if(!too_few_args && !too_few_args_whitespace){
            invalid_argument = true;
            cout << "Error: invalid argument" << endl;

            //peek_checker(lineStream, invalid_argument);
        }
    }
}

void peek_checker(stringstream &lineStream, bool &invalid_argument){
    if (lineStream.peek() != ' ' && !lineStream.eof() && !invalid_argument){
        cout << "Error: invalid argument" << endl;
        invalid_argument = true;
    }
}
