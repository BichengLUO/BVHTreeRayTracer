//
//  config.h
//  Raytra
//
//  Created by Eagle Sky on 23/10/2017.
//  Copyright © 2017 Eagle Sky. All rights reserved.
//

#ifndef config_h
#define config_h

class Config
{
public:
    static Config& getInstance()
    {
        static Config instance;
        return instance;
    }
    Config(const Config &) = delete;
    void operator=(const Config &) = delete;
    
    bool bboxesEnabled() {return _enable_bboxes;}
    void enableBBoxes(bool flag) {_enable_bboxes = flag;}
private:
    Config() {}
    bool _enable_bboxes;
};

#endif /* config_h */
