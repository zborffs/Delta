#ifndef DELTA_MODEL_HPP
#define DELTA_MODEL_HPP

class Model {
public:
    virtual ~Model() = default;
    virtual int step(int state_vector, int control_vector) = 0;
};

#endif //DELTA_MODEL_HPP
