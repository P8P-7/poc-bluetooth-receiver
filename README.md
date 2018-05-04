# Bluetooth Reciever
*Proof of Concept*

## Build steps

### Raspbian (based on Debian 9 "Stretch")
1. Install packages
   ```bash
   sudo apt-get install libtool pkg-config build-essential autoconf automake git cmake autogen graphviz libboost-dev libgtest-dev libtool python-sip-dev doxygen libbluetooth-dev
   ```

2. Build and install [libzmq](https://github.com/zeromq/libzmq).
    ```bash
    curl -L https://github.com/zeromq/libzmq/releases/download/v4.2.5/zeromq-4.2.5.tar.gz -o zeromq-4.2.5.tar.gz
    tar -xvzf zeromq-4.2.5.tar.gz
    cd zeromq-4.2.5
    sed -i -e 's/include(ClangFormat)/\#include(ClangFormat)/g' CMakeLists.txt
    mkdir build && cd build
    cmake -D ZMQ_BUILD_TESTS=OFF ..
    sudo make -j4 install
    ```

3. Build and install [cppzmq](https://github.com/zeromq/cppzmq).
   ```bash
   git clone https://github.com/zeromq/cppzmq.git
   cd cppzmq
   mkdir build && cd build
   cmake -D CPPZMQ_BUILD_TESTS=OFF ..
   sudo make -j4 install
   ```

4. Compile the GTest library object files and copy libgtest.a and libgtest_main.a into your /usr/lib/ directory.
   ```bash
   cd /usr/src/gtest
   sudo cmake CMakeLists.txt
   sudo make
   sudo cp *.a /usr/lib
   ```

5. Build Bluetooth Reciever.
   ```bash
   git clone https://github.com/hidder11/poc_bluetooth_reciever.git
   cd poc_bluetooth_reciever
   cmake .
   make
   ```

## Usage
1. Configure Serial Bluetooth.
   ```bash
   sudo nano /etc/systemd/system/dbus-org.bluez.service
   ```
   add the -C flag to the end of the ExecStart line and add the sp profile underneith.
   ```bash
   ExecStart=/usr/lib/bluetooth/bluetoothd -C
   ExecStartPost=/usr/bin/sdptool add SP
   ```
   Now pair and trust your bluetooth device like the controller.

2. Connect bluetooth device
   Make the raspberry able to accept a bluetooth connection
   ```bash
   sudo rfcomm watch hci0
   ```
   Now connect your bluetooth device to the raspberry.

3. Start the broker and subscriber.

4. Start the publisher.
   ```bash
   sudo ./Publisher [device name]
   ```
   optionaly add the device name as a parameter. The default is /dev/rfcomm0
