#ifndef KITTIES_H
#define KITTIES_H

#include "sfcomponent.h"

class Kitties : public SceneObject
{
public:

    virtual void initialize();
    virtual void tick(float ticktime);
    virtual void render(float ticktime);

    // Render any postprocessing effects. The default implementation updates the
    // matrix with the objects location and rotation and then calls postprocess
    // on the child_nodes
    virtual void postprocess(float ticktime);

    // Return the axis_aligned bounding box of just this object (not its children)
    // the default is nothing (width/height zero). The returned value should be in
    // the object's coordinate space. It is up to the caller to multiply it by
    // the matrix
    virtual QRect get_intrinsic_aabbox();
  
private:
    
};

#endif // KITTIES_H
