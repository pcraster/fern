#pragma once


namespace ranally {
namespace io {

//! Class that encapsulates the configuration of the HDF5 library for the HDF5 client.
/*!
  In case an error occurs in one of HDF5's C++ API calls, an exception is
  thrown, instead of a message being printed.
*/
class HDF5Client
{

    friend class HDF5ClientTest;

public:

                   HDF5Client          (HDF5Client const&)=delete;

    HDF5Client&    operator=           (HDF5Client const&)=delete;

    virtual        ~HDF5Client         ();

protected:

                   HDF5Client          ();

private:

};

} // namespace io
} // namespace ranally