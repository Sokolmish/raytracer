#ifndef __OBJMODEL_H__
#define __OBJMODEL_H__

#include "objects.hpp"

typedef std::string::const_iterator strIter;

class ObjModel {
private:
    std::vector<Vec3f> vs, vns;
    std::vector<Vec2f> vts;
    std::vector<VolumeObj*> objects;

    void scanLine(strIter &it, const strIter &end);

    void scanVertex(strIter &it, const strIter &end);
    void scanFace(strIter &it, const strIter &end);

    int scanInt(strIter &it, const strIter &end);
    float scanNumber(strIter &it, const strIter &end);
    void scanComment(strIter &it, const strIter &end);
    void scanSpaces(strIter &it, const strIter &end);
    int scanWhole(strIter &it, const strIter &end);
    float scanFractPart(strIter &it, const strIter &end);

    bool checkAssertion(bool expr, std::string msg);
public:
    ObjModel(std::string str);
};

#endif