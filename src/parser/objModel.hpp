#ifndef __OBJMODEL_H__
#define __OBJMODEL_H__

#include "../objects.hpp"
#include <string>
#include <map>

typedef std::string::const_iterator strIter;

class mtlParser {
private:
    std::map<std::string, Material> mats;

    std::string curName;
    Material curMaterial;
    
    void scanLine(strIter &it, const strIter &end);

    void scanNew(strIter &it, const strIter &end);
    void scanColor(strIter &it, const strIter &end);
    void scanScalar(strIter &it, const strIter &end);
    // void scanMap(strIter &it, const strIter &end);

    friend class ObjModel;
public:
    mtlParser(std::string path);
};


class ObjModel {
private:
    std::string root;

    Vec3f center;
    Mat3f transform;

    std::vector<Vec3f> vs, vns;
    std::vector<Vec2f> vts;
    std::vector<VolumeObj*> objects;
    Material curMat;

    std::map<std::string, Material> mats;

    void scanLine(strIter &it, const strIter &end);

    void scanVertex(strIter &it, const strIter &end);
    void scanFace(strIter &it, const strIter &end);
    void scanMtl(strIter &it, const strIter &end);

    bool checkAssertion(bool expr, std::string msg);
public:
    ObjModel(const Vec3f &center, const Mat3f &trans, const std::string &path);

    const std::vector<VolumeObj*>& getObjects() const {
        return this->objects;
    };
};


bool isDigit(char ch);
bool isAlphabetic(char ch);
bool isSpecSymb(char ch);

void scanComment(strIter &it, const strIter &end);
void scanSpaces(strIter &it, const strIter &end);

int scanInt(strIter &it, const strIter &end);
float scanNumber(strIter &it, const strIter &end);
int scanWhole(strIter &it, const strIter &end);
float scanFractPart(strIter &it, const strIter &end);

bool checkWord(strIter &it, const strIter &end, const std::string &word);

std::string scanIdent(strIter &it, const strIter &end);

#endif