//
//  MouseController.hpp
//  ball
//
//  Created by Dcalsky on 16/5/30.
//
//

#ifndef MouseController_hpp
#define MouseController_hpp

class MouseController {
private:
    MouseController();
    static MouseController * _mouseController;
public:
    static MouseController * getInstance()
    {
        if( _mouseController == nullptr) {
            _mouseController = new MouseController();
        }
        return _mouseController;
    };
};

#endif /* MouseController_hpp */
