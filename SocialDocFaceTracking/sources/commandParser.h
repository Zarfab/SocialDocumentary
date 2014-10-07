/**
 * \file CommandParser.h
 * \brief Parseur de cmd argv.
 * \author Julien Leroy
 * \version 0.1
 * \date 09 Juillet 2012
 *
 * Programme pour la lecture de paramètre en argv 
 *
 */
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

class CommandParser {
public:

    static void split(std::vector<std::string> &values, string str) {
        istringstream ss(str);
        while (!ss.eof()) {
            string x; // here's a nice, empty string
            getline(ss, x, ','); // try to read the next field into it
            //cout << x << endl;      // print it out, even if we already hit EOF
            values.push_back(x);
        }
    }

    /** \brief Finds whether the argument with name "argument_name" is in the argument list "argv".
     * An example for a widely used switch argument is the "-r" flag for unix commands that indicates whether
     * the command should run recursively or not.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] argument_name the string value to search for
     * \return true if argument was found, false otherwise
     * \note find_switch is simply returning find_argument != -1.
     */
    static bool
    find_switch(int argc, char** argv, const char* argument_name) {
        return (find_argument(argc, argv, argument_name) != -1);
    }

    /** \brief Finds the position of the argument with name "argument_name" in the argument list "argv"
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] argument_name the string value to search for
     * \return index of found argument or -1 of arguments does not appear in list
     */
    static int
    find_argument(int argc, char** argv, const char* argument_name) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if (strcmp(argv[i], argument_name) == 0) {
                return (i);
            }
        }
        return (-1);
    }

    /** \brief Template version for parsing arguments. Template parameter needs to have input stream operator overloaded!
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] argument_name the name of the argument to search for
     * \param[out] value The value of the argument
     * \return index of found argument or -1 of arguments does not appear in list
     */
    template<typename Type> int
    parse(int argc, char** argv, const char* argument_name, Type& value) {
        int index = find_argument(argc, argv, argument_name) + 1;

        if (index > 0 && index < argc) {
            std::istringstream stream;
            stream.clear();
            stream.str(argv[index]);
            stream >> value;
        }

        return (index - 1);
    }

    /** \brief Parse for a specific given command line argument. Returns the value
     * sent as a string.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] val the resultant value
     */
    static int
    parse_argument(int argc, char** argv, const char* str, std::string &val) {
        int index = find_argument(argc, argv, str) + 1;
        if (index > 0 && index < argc)
            val = argv[index];

        return index - 1;
    }

    /** \brief Parse for a specific given command line argument. Returns the value
     * sent as a boolean.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] val the resultant value
     */
    static int
    parse_argument(int argc, char** argv, const char* str, bool &val) {
        int index = find_argument(argc, argv, str) + 1;

        if (index > 0 && index < argc)
            val = atoi(argv[index]) == 1;

        return (index - 1);
    }

    /** \brief Parse for a specific given command line argument. Returns the value
     * sent as a double.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] val the resultant value
     */
    static int
    parse_argument(int argc, char** argv, const char* str, float &val) {
        int index = find_argument(argc, argv, str) + 1;

        if (index > 0 && index < argc)
            val = static_cast<float> (atof(argv[index]));

        return (index - 1);
    }

    /** \brief Parse for a specific given command line argument. Returns the value
     * sent as a double.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] val the resultant value
     */
    static int
    parse_argument(int argc, char** argv, const char* str, double &val) {
        int index = find_argument(argc, argv, str) + 1;

        if (index > 0 && index < argc)
            val = atof(argv[index]);

        return (index - 1);
    }

    /** \brief Parse for a specific given command line argument. Returns the value
     * sent as an int.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] val the resultant value
     */
    static int
    parse_argument(int argc, char** argv, const char* str, int &val) {
        int index = find_argument(argc, argv, str) + 1;

        if (index > 0 && index < argc)
            val = atoi(argv[index]);

        return (index - 1);
    }

    /** \brief Parse for a specific given command line argument. Returns the value
     * sent as an unsigned int.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] val the resultant value
     */
    static int
    parse_argument(int argc, char** argv, const char* str, unsigned int &val) {
        int index = find_argument(argc, argv, str) + 1;

        if (index > 0 && index < argc)
            val = atoi(argv[index]);

        return (index - 1);
    }

    /** \brief Parse for a specific given command line argument. Returns the value
     * sent as an int.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] val the resultant value
     */
    static int
    parse_argument(int argc, char** argv, const char* str, char &val) {
        int index = find_argument(argc, argv, str) + 1;

        if (index > 0 && index < argc)
            val = argv[index][0];

        return (index - 1);
    }

    /** \brief Parse for specific given command line arguments (2x values comma
     * separated). Returns the values sent as doubles.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] f the first output value
     * \param[out] s the second output value
     * \param[in] debug whether to print debug info or not
     */
    static int
    parse_2x_arguments(int argc, char** argv, const char* str, float &f, float &s, bool debug = true) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                // look for ',' as a separator
                std::vector<std::string> values;
                //            boost::split(values, argv[i], boost::is_any_of(","), boost::token_compress_on);
                split(values, argv[i]);
                if (values.size() != 2) {
                    printf("[parse_2x_arguments] Number of values for %s (%zu) different than 2!\n", str, values.size());
                    return (-2);
                }
                f = static_cast<float> (atof(values.at(0).c_str()));
                s = static_cast<float> (atof(values.at(1).c_str()));
                return (i - 1);
            }
        }
        return (-1);
    }

    /** \brief Parse for specific given command line arguments (2x values comma
     * separated). Returns the values sent as doubles.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] f the first output value
     * \param[out] s the second output value
     * \param[in] debug whether to print debug info or not
     */
    static int
    parse_2x_arguments(int argc, char** argv, const char* str, string &f, string &s, bool debug = true) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                // look for ',' as a separator
                std::vector<std::string> values;
                //            boost::split(values, argv[i], boost::is_any_of(","), boost::token_compress_on);
                split(values, argv[i]);
                if (values.size() != 2) {
                    printf("[parse_2x_arguments] Number of values for %s (%zu) different than 2!\n", str, values.size());
                    return (-2);
                }
                f = values[0];
                s = values[1];
                return (i - 1);
            }
        }
        return (-1);
    }

    /** \brief Parse for specific given command line arguments (2x values comma
     * separated). Returns the values sent as doubles.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] f the first output value
     * \param[out] s the second output value
     * \param[in] debug whether to print debug info or not
     */
    static int
    parse_2x_arguments(int argc, char** argv, const char* str, double &f, double &s, bool debug = true) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                // look for ',' as a separator
                std::vector<std::string> values;
                //            boost::split(values, argv[i], boost::is_any_of(","), boost::token_compress_on);
                split(values, argv[i]);
                if (values.size() != 2) {
                    printf("[parse_2x_arguments] Number of values for %s (%zu) different than 2!\n", str, values.size());
                    return (-2);
                }
                f = atof(values.at(0).c_str());
                s = atof(values.at(1).c_str());
                return (i - 1);
            }
        }
        return (-1);
    }

    /** \brief Parse for specific given command line arguments (2x values comma
     * separated). Returns the values sent as ints.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] f the first output value
     * \param[out] s the second output value
     * \param[in] debug whether to print debug info or not
     */
    static int
    parse_2x_arguments(int argc, char** argv, const char* str, int &f, int &s, bool debug = true) {

        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                // look for ',' as a separator
                std::vector<std::string> values;
                //            boost::split(values, argv[i], boost::is_any_of(","), boost::token_compress_on);
                split(values, argv[i]);

                if (values.size() != 2) {
                    printf("[parse_2x_arguments] Number of values for %s (%zu) different than 2!\n", str, values.size());
                    return (-2);
                }
                f = atoi(values.at(0).c_str());
                s = atoi(values.at(1).c_str());
                return (i - 1);
            }
        }
        return (-1);
    }

    /** \brief Parse for specific given command line arguments (3x values comma
     * separated). Returns the values sent as doubles.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] f the first output value
     * \param[out] s the second output value
     * \param[out] t the third output value
     * \param[in] debug whether to print debug info or not
     */
    static int
    parse_3x_arguments(int argc, char** argv, const char* str, float &f, float &s, float &t, bool debug = true) {
        return 0;
    }

    /** \brief Parse for specific given command line arguments (3x values comma
     * separated). Returns the values sent as doubles.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] f the first output value
     * \param[out] s the second output value
     * \param[out] t the third output value
     * \param[in] debug whether to print debug info or not
     */
    static int
    parse_3x_arguments(int argc, char** argv, const char* str, double &f, double &s, double &t, bool debug = true) {
        return 0;
    }

    /** \brief Parse for specific given command line arguments (3x values comma
     * separated). Returns the values sent as ints.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] f the first output value
     * \param[out] s the second output value
     * \param[out] t the third output value
     * \param[in] debug whether to print debug info or not
     */
    static int
    parse_3x_arguments(int argc, char** argv, const char* str, int &f, int &s, int &t, bool debug = true) {
        return 0;
    }

    /** \brief Parse for specific given command line arguments (3x values comma
     * separated). Returns the values sent as doubles.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] v the vector into which the parsed values will be copied
     */
    static int
    parse_x_arguments(int argc, char** argv, const char* str, std::vector<double>& v) {
        return 0;
    }

    /** \brief Parse for specific given command line arguments (N values comma
     * separated). Returns the values sent as ints.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] v the vector into which the parsed values will be copied
     */
    static int
    parse_x_arguments(int argc, char** argv, const char* str, std::vector<float>& v) {
        return 0;
    }

    /** \brief Parse for specific given command line arguments (N values comma
     * separated). Returns the values sent as ints.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] v the vector into which the parsed values will be copied
     */
    static int
    parse_x_arguments(int argc, char** argv, const char* str, std::vector<int>& v) {
        return 0;
    }

    /** \brief Parse for specific given command line arguments (multiple occurances
     * of the same command line parameter). Returns the values sent as a vector.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] values the resultant output values
     */
    static bool
    parse_multiple_arguments(int argc, char** argv, const char* str, std::vector<int> &values) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                int val = atoi(argv[i]);
                values.push_back(val);
            }
        }
        if (values.size() == 0)
            return (false);
        else
            return (true);
    }

    /** \brief Parse for specific given command line arguments (multiple occurances
     * of the same command line parameter). Returns the values sent as a vector.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] values the resultant output values
     */
    static bool
    parse_multiple_arguments(int argc, char** argv, const char* str, std::vector<float> &values) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                float val = static_cast<float> (atof(argv[i]));
                values.push_back(val);
            }
        }
        if (values.size() == 0)
            return (false);
        else
            return (true);
    }

    /** \brief Parse for specific given command line arguments (multiple occurances
     * of the same command line parameter). Returns the values sent as a vector.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] values the resultant output values
     */
    static bool
    parse_multiple_arguments(int argc, char** argv, const char* str, std::vector<double> &values) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                double val = atof(argv[i]);
                values.push_back(val);
            }
        }
        if (values.size() == 0)
            return (false);
        else
            return (true);
    }

    /** \brief Parse for a specific given command line argument (multiple occurences
     * of the same command line parameter). Returns the value sent as a vector.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the string value to search for
     * \param[out] values the resultant output values
     */
    static bool
    parse_multiple_arguments(int argc, char** argv, const char* str, std::vector<std::string> &values) {
        for (int i = 1; i < argc; ++i) {
            // Search for the string
            if ((strcmp(argv[i], str) == 0) && (++i < argc)) {
                values.push_back(std::string(argv[i]));
            }
        }
        if (values.size() == 0)
            return (false);
        else
            return (true);
    }

    /** \brief Parse for specific given command line arguments (multiple occurances
     * of 2x argument groups, separated by commas). Returns 2 vectors holding the
     * given values.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] values_f the first vector of output values
     * \param[out] values_s the second vector of output values
     */
    static bool
    parse_multiple_2x_arguments(int argc, char** argv, const char* str,
            std::vector<double> &values_f,
            std::vector<double> &values_s) {
        return false;
    }

    /** \brief Parse for specific given command line arguments (multiple occurances
     * of 3x argument groups, separated by commas). Returns 3 vectors holding the
     * given values.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] str the command line argument to search for
     * \param[out] values_f the first vector of output values
     * \param[out] values_s the second vector of output values
     * \param[out] values_t the third vector of output values
     */
    static bool
    parse_multiple_3x_arguments(int argc, char** argv, const char* str,
            std::vector<double> &values_f,
            std::vector<double> &values_s,
            std::vector<double> &values_t) {
        return false;
    }

    /** \brief Parse command line arguments for file names. Returns a vector with
     * file names indices.
     * \param[in] argc the number of command line arguments
     * \param[in] argv the command line arguments
     * \param[in] ext the extension to search for
     */
    static std::vector<int>
    parse_file_extension_argument(int argc, char** argv, const std::string &extension) {
        std::vector<int> indices;
        for (int i = 1; i < argc; ++i) {
            std::string fname = std::string(argv[i]);
            std::string ext = extension;

            // Needs to be at least 4: .ext
            if (fname.size() <= 4)
                continue;

            // For being case insensitive
            //    std::transform (fname.begin (), fname.end (), fname.begin (), tolower);
            //    std::transform (ext.begin (), ext.end (), ext.begin (), tolower);

            // Check if found
            std::string::size_type it;
            if ((it = fname.rfind(ext)) != std::string::npos) {
                // Additional check: we want to be able to differentiate between .p and .png
                if ((ext.size() - (fname.size() - it)) == 0)
                    indices.push_back(i);
            }
        }
        return (indices);
    }

};
#endif      

