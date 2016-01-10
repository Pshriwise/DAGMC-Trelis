/*!
 * \file   MyObserver.cpp
 * \author Michael Plooster
 * \date   4 Aug 2014
 */

#include "MyObserver.hpp"
#include "hdf5.h"

#include <iostream>

MyObserver::MyObserver()
{
  // Let Trelis know that this class will be observing events.
  register_observer();
}

MyObserver::~MyObserver()
{
  // Let Trelis know this class will no longer be observing events.
  unregister_observer();
}

void MyObserver::notify_cub_file_read(std::string filename)
{
  // Here is an example reading our data from an HDF5 format .cub file. For
  // more info (and better examples), visit http://www.hdfgroup.org/HDF5/doc/index.html

  // Try to open the given file with read-only access
  hid_t file = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  // Check that we have a valid hdf5 file
  if(file < 0)
    return;

  // Open the unique group where we saved our data
  hid_t group = H5Gopen(file, "/MyCompanyName", H5P_DEFAULT);

  // Get our dataset. Note that I can use a relative path here because
  // I am opening the dataset from within "group".
  hid_t dataset = H5Dopen(group, "MyArray", H5P_DEFAULT);

  // Read our data
  int my_data[10];
  H5Dread(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, my_data);

  // Close references when we are done using them.
  H5Dclose(dataset);
  H5Gclose(group);
  H5Fclose(file);

  // Just for fun, print out the data we just read.
  for(int i = 0; i < 10; i++)
    std::cout << my_data[i] << " ";
  std::cout << std::endl;
}

void MyObserver::notify_cub_file_saved(std::string filename)
{
  // Here is example writing out some data to an HDF5 format .cub file. For
  // more info (and better examples), visit http://www.hdfgroup.org/HDF5/doc/index.html

  // Open the file. At this point, Trelis should have created the file already
  // and we just need to open it to write piece(s) of data.
  hid_t file = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

  // Check that we have a valid hdf5 file
  if(file < 0)
    return;

  // Create a unique group to store my data. The group name should be something
  // that is unlikely to be used by anyone else who may be writing to the file.
  hid_t group = H5Gcreate(file, "/MyCompanyName",
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  // Setup our example data
  int my_data[] = {1,2,3,4,5,6,7,8,9,0};

  // Setup a dataspace and dataset to write out the data to "/MyCompanyName/MyArray"
  hsize_t dims[] = {10};
  hid_t dataspace = H5Screate_simple(1, dims, NULL);
  hid_t datatype = H5Tcopy(H5T_NATIVE_INT);
  hid_t dataset = H5Dcreate(group, "MyArray", datatype, dataspace,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, my_data);

  // Close references when we are done using them
  H5Dclose(dataset);
  H5Tclose(datatype);
  H5Sclose(dataspace);
  H5Gclose(group);
  H5Fclose(file);
}
