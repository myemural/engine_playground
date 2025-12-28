//
// Created by oguzh on 28.12.2025.
//

#ifndef HIT_INFO_H
#define HIT_INFO_H



class HitInfo {
    bool state;
    double t;
public:
    HitInfo();
    bool hit() const;
    void hit(bool s);
    double time() const;
    void time(double t_hit);

};



#endif //HIT_INFO_H
