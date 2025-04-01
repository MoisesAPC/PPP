#ifndef COMBOBOXDATA_H
#define COMBOBOXDATA_H

/**
 * @file ComboBoxData.h
 *
 * This header file defines the typedef "ComboBoxData", which associates
 * a string (displayed in a combo box interface element), with the actual,
 * numeric value that will get selected when clicking on said option.
 *
 * This is done using a vector of maps, and each map has a string and a numeric value associated to it.
 *
 * @author Moisés Antonio Pestano Castro
 */

#include <map>
#include <vector>
#include <string>

namespace Ui {

typedef std::vector<std::map<std::string, int>> ComboBoxData;

}

#endif
