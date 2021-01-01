//
// Created by Zach Bortoff on 12/29/20.
//

#ifndef DELTA_APP_INTERFACE_HPP
#define DELTA_APP_INTERFACE_HPP

class AppInterface {
public:
    virtual ~AppInterface() = default;
    virtual int engine_data() = 0;
};

#endif // DELTA_APP_INTERFACE_HPP
