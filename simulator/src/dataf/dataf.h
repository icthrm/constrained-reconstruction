#ifndef DATAFORMAT_H__
#define DATAFORMAT_H__

#include <vector>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <string>
#include <climits>
#include <cmath>
#include "keypoint.h"
#include "util/exception.h"

namespace util {

/** @brief FeatureStack; the output of sift process*/
class FeatureStack {
private:
    int size;
    std::vector<util::feature>* vfeatp;
    int width, height;
    float ratio;

public:
    FeatureStack():size(0), vfeatp(NULL), ratio(0.5), width(1024), height(1024) {}

    FeatureStack(int _size):size(_size) {
        vfeatp = new std::vector<util::feature>[size];
    }

    ~FeatureStack() {
        if(vfeatp) {
            delete [] vfeatp;
        }
    }

    void ReSize(int  _size) {
        if(size != _size) {
            delete [] vfeatp;
            size = _size;
            vfeatp = new std::vector<util::feature>[size];
        }
    }

    void Clear() {
        delete [] vfeatp;
        vfeatp = new std::vector<util::feature>[size];
    }

    void SetWxH(int _width, int _height) {
        width = _width;
        height = _height;
    }
    void SetRatio(float r) {
        ratio = r;
    }

    void Release() {
        if(vfeatp) {
            delete [] vfeatp;
        }
        vfeatp = NULL;
    }

    std::vector<util::feature>& V(int index) {
        assert(index>=0&&index<size);
        return vfeatp[index];
    }
    const std::vector<util::feature>& V(int index) const {
        assert(index>=0&&index<size);
        return vfeatp[index];
    }

    int Size() const {
        return size;
    }
    int Width() const {
        return width;
    }
    int Height() const {
        return height;
    }
    float Ratio() const {
        return ratio;
    }

    void WriteStackByFolder(const char* folderpath) const;
    void ReadStackByFolder(const char* folderpath);
};


typedef _point point2d;

/** @brief Stack of fiducial markers*/
class FiducialStack {
private:
    int size;
    std::vector<util::point2d>* vfidp;
    int width, height;
    float ratio;

public:
    FiducialStack():size(0), vfidp(NULL), width(-1), height(-1), ratio(1) {}

    FiducialStack(int _size):size(_size) {
        vfidp = new std::vector<util::point2d>[size];
    }

    ~FiducialStack() {
        if(vfidp) {
            delete [] vfidp;
        }
    }

    void ReSize(int  _size) {
        if(size != _size) {
            delete [] vfidp;
            size = _size;
            vfidp = new std::vector<util::point2d>[size];
        }
    }
    void SetRatio(float r) {
        ratio = r;
    }

    void Clear() {
        delete [] vfidp;
        vfidp = new std::vector<util::point2d>[size];
    }

    void SetWxH(int _width, int _height) {
        width = _width;
        height = _height;
    }

    void Release() {
        if(vfidp) {
            delete [] vfidp;
        }
        vfidp = NULL;
    }

    std::vector<util::point2d>& V(int index) {
        assert(index>=0&&index<size);
        return vfidp[index];
    }
    const std::vector<util::point2d>& V(int index) const {
        assert(index>=0&&index<size);
        return vfidp[index];
    }

    int Size() const {
        return size;
    }
    int Width() const {
        return width;
    }
    int Height() const {
        return height;
    }
    float Ratio() const {
        return ratio;
    }

    void WriteFidsByFile(const char* filename) const;
    bool ReadFidsByFile(const char* filename);
};

typedef std::pair<util::_point, util::_point> pair;

struct img_match {
    int idx1, idx2;		//the index of first image and sencond image; which is matched
    std::vector<pair> pairs;
    size_t size() const {
        return pairs.size();
    }
};

/** @brief MatchPairStack; the output of keymatch process*/
class ImgMatchVector {
private:
    std::vector<img_match>* match_vector;
public:
    ImgMatchVector()
    {
        match_vector = new std::vector<img_match>();
    }

    ~ImgMatchVector()
    {
        if(match_vector) {
            delete match_vector;
        }
    }

    void Clear()
    {
        match_vector->clear();
    }

    void Release()
    {
        if(match_vector) {
            delete match_vector;
        }
        match_vector = NULL;
    }

    int Size() const {
        return match_vector->size();
    }

    img_match& operator[](int index)
    {
        assert(index>=0&&index<match_vector->size());
        return (*match_vector)[index];
    }

    const img_match& operator[](int index) const
    {
        assert(index>=0&&index<match_vector->size());
        return (*match_vector)[index];
    }

    img_match& V(int index) {
        assert(index>=0&&index<match_vector->size());
        return (*match_vector)[index];
    }
    const img_match& V(int index) const {
        assert(index>=0&&index<match_vector->size());
        return (*match_vector)[index];
    }

    img_match& MallocNewMatch()
    {
        img_match data;
        match_vector->push_back(data);
        return (*match_vector)[match_vector->size()-1];
    }

    void PushBack(const img_match& data)
    {
        match_vector->push_back(data);
    }
    
    img_match* GetMatchSetWithIdx(int idx1, int idx2, bool& noex){							//!!!!!!!!!!!!!!!!!!!!!!!!!!!
		for(int i = 0; i < match_vector->size(); i++){
			if((*match_vector)[i].idx1 == idx1 && (*match_vector)[i].idx2 == idx2){
				noex = true;
				return &((*match_vector)[i]);
			}
			if((*match_vector)[i].idx1 == idx2 && (*match_vector)[i].idx2 == idx1){
				noex = false;
				return &((*match_vector)[i]);
			}
		}
		return NULL;
	}

    static void ReadPairs(img_match* pairs, std::istream& in);
    void CoordinateTransform(int width, int height);
    void PreRotate(float angle);		//angle in degree
    void PrintPairs(int index, std::ostream& o) const;
    void WriteVectorByFolder(const char* folderpath) const;
    void ReadVectorByFolder(const char* folderpath);
};

/** @brief the class node stores the point in an progect image and the relations between the points*/
struct _info {
    float angle;
    int z;
};

#define ISEQUAL(x, y)		fabs((x)-(y)) < 0.001

class NodePlane {
    friend class TrackSpace;
private:
    struct _node {
        _point p;
        _node* pre;
        _node* nxt;
        _node* _m2234;
        void* ref;
        long extra_idx;
        _node() {
            extra_idx = -1;
        }
        _node(const _point& pp){
			p = pp;
            extra_idx = -1;
        }
    };
private:
    struct _lnode {
        float& x;
        _lnode* pre;
        _lnode* nxt;
        _node* ptr;
		
		_lnode(_node* nnode):ptr(nnode), x(nnode->p.x){}
		
    };
private:
	_node xbegin;
	_node xend;
    _lnode xlist_begin;
    _lnode xlist_end;
    void* information;
    _lnode* _m2307;
    _node* pre_visited;

private:
    _node* BaseAddNode(const _point& point);

public:
    class NodeReader {
        friend class NodePlane;
    private:
        _lnode* clptr;
        _lnode* clnullptr;
        _node* cnptr;
    private:
        NodeReader() {}
    public:
        _node* Next()
        {
            if(clptr == clnullptr){
                return NULL;
            }

            if(cnptr == NULL || clptr->ptr->p.x == INT_MIN) {
                clptr = clptr->nxt;
                cnptr = clptr->ptr;
            }
            else {
                cnptr = cnptr->_m2234?cnptr->_m2234:(clptr = clptr->nxt)->ptr;
            }
            
            if(cnptr->p.x == INT_MAX){
				return NULL;
            }
            
            return cnptr;
        }
    };
public:
    NodePlane():xbegin(),xend(),xlist_begin(&xbegin),xlist_end(&xend){
        xlist_begin.pre = NULL;
        xlist_begin.nxt = &xlist_end;
        xlist_begin.x = INT_MIN;
        xlist_end.pre = &xlist_begin;
        xlist_end.nxt = NULL;
        xlist_end.x = INT_MAX;
        _m2307 = &xlist_begin;
        pre_visited = NULL;
    }
    ~NodePlane();
    void SetInfo(void* info) {
        information = info;
    }

    _node* AddNode(int x, int y) {
        return AddNode(_point(x,y));
    }

    _node* AddNode(const _point& point);

    static void Link(_node* pnode1, _node* pnode2)
    {
        _node* tmp;
        if(!pnode1->nxt) {
            tmp = pnode1;
        }
        else {
            tmp = pnode1;
            while(tmp->nxt && ((_info*)(tmp->nxt->ref))->z < ((_info*)(pnode2->ref))->z) {
                tmp = tmp->nxt;
            }
        }

        _node* tmp2;

        if(!pnode2->pre) {
            tmp2 = pnode2;
        }
        else {
            tmp2 = pnode2;
            while(tmp2->pre) {
                tmp2 = tmp2->pre;
				if(tmp2 == pnode1){
					return;			//the path has already covered pnode1;
				}
            }
        }

//         tmp->nxt = tmp2;
//         tmp2->pre = tmp;
        
        if(!tmp->nxt && ((_info*)(tmp->ref))->z < ((_info*)(tmp2->ref))->z) {
            tmp->nxt = tmp2;
            tmp2->pre = tmp;
        }
        else if(!tmp->nxt && ((_info*)(tmp->ref))->z > ((_info*)(tmp2->ref))->z
                && tmp->pre && ((_info*)(tmp->pre->ref))->z < ((_info*)(tmp2->ref))->z
                && tmp2->nxt && ((_info*)(tmp->ref))->z < ((_info*)(tmp2->nxt->ref))->z) {
            tmp->nxt = tmp2->nxt;
            tmp2->nxt->pre = tmp;
            tmp->pre->nxt = tmp2;
            tmp2->pre = tmp->pre;
            tmp2->nxt = tmp;
            tmp->pre = tmp2;
        }
        else if((((_info*)(tmp->ref))->z < ((_info*)(pnode2->ref))->z)
                && tmp->nxt && (((_info*)(tmp->nxt->ref))->z > ((_info*)(pnode2->ref))->z) && pnode2->pre == NULL) {
            pnode2->nxt = tmp->nxt;
            tmp->nxt->pre = pnode2;
            tmp->nxt = pnode2;
            pnode2->pre = tmp;
        }
        else{
// 			if(!(tmp->p.x == tmp2->p.x && tmp->p.y == tmp2->p.y)){
// 				std::cout<<((_info*)(tmp->ref))->z<<" "<<((_info*)(tmp2->ref))->z<<std::endl;
// 			}
		}
    }

    NodeReader GetReader()
    {
        NodeReader nreader;
        nreader.clptr = &xlist_begin;
        nreader.cnptr = xlist_begin.ptr;
        nreader.clnullptr = &xlist_end;
        return nreader;
    }

public:
    int Print(std::ostream& o) const;
};

union pt2 {
    struct {
        double x;
        double y;
    };
    double v[2];
    const pt2& operator = (const pt2& pt_) {
        x = pt_.x;
        y = pt_.y;
        return *this;
    }
};

union pt3 {
    struct {
        double x;
        double y;
        double z;
    };
    double v[3];

    const pt3& operator = (const pt3& pt_) {
        x = pt_.x;
        y = pt_.y;
        z = pt_.z;
        return *this;
    }
};

class TrackSpace {
private:
    util::NodePlane* xyplane_array;	//xlist stores the x coordinate data;and, xyplane_array sotres the z coordinate data
    std::vector<util::NodePlane::_node*>* nodes_v;
    std::vector<pt3> pt3_v;
    _info* info_array;			//info_array sotres the z coordinate data
    int size;

public:
    class Iterator {
        friend class TrackSpace;
        friend class TrackNode;
    private:
        std::vector<util::NodePlane::_node*>* pnodes;
        std::vector<pt3>& pt3_v;
        int index;
    private:
        Iterator(TrackSpace& trspce):pt3_v(trspce.pt3_v) {}

    public:
        const float& X()
        {
            return (*pnodes)[index]->p.x;
        }

        const float& Y()
        {
            return (*pnodes)[index]->p.y;
        }

        const int& Z()
        {
            return ((_info*)(*pnodes)[index]->ref)->z;
        }

        const float& Angle()
        {
            return ((_info*)(*pnodes)[index]->ref)->angle;
        }

        /** @brief ++:like i++*/
        Iterator operator++(int)
        {
            Iterator tmp = *this;
            index++;
            return tmp;
        }

        /** @brief ++:like ++i*/
        Iterator& operator ++()
        {
            index++;
            return *this;
        }

        /** @brief --:like i--*/
        Iterator operator--(int)
        {
            Iterator tmp = *this;
            index--;
            return tmp;
        }

        /** @brief --:like --i*/
        Iterator& operator --()
        {
            index--;
            return *this;
        }

        bool operator ==(const Iterator& it) const
        {
            return pnodes == it.pnodes && index == it.index;
        }

        bool operator !=(const Iterator& it) const
        {
            return pnodes != it.pnodes || index != it.index;
        }

        const Iterator& operator =(const Iterator& it)
        {
            pnodes = it.pnodes;
            index = it.index;
            return *this;
        }

        bool IsNULL() {
            return index >= (*pnodes).size()||index < 0;
        }

        void SetExtraAsZVectorIndex(long idx) {
            (*pnodes)[index]->extra_idx = idx;
        }
    };

    class TrackNode {
    private:
        util::NodePlane::_node* pnode;
        std::vector<pt3>& pt3_v;
    public:
        TrackNode(const Iterator& it):pt3_v(it.pt3_v)
        {
            pnode = (*(it.pnodes))[it.index];
        }

        TrackNode(const TrackNode& tracknode):pnode(tracknode.pnode), pt3_v(tracknode.pt3_v) {}

        bool IsBegin() const
        {
            return pnode->pre == NULL;
        }

        bool IsRBegin() const
        {
            return pnode->nxt == NULL;
        }

        const float& X() const
        {
            return pnode->p.x;
        }

        const float& Y() const
        {
            return pnode->p.y;
        }

        const int& Z() const
        {
            return ((_info*)(pnode->ref))->z;
        }

        const float& Angle() const
        {
            return ((_info*)(pnode->ref))->angle;
        }

        TrackNode operator++(int)
        {
            TrackNode tmp = *this;
            pnode = pnode->nxt;
            return tmp;
        }

        /** @brief ++:like ++i*/
        TrackNode& operator ++()
        {
            pnode = pnode->nxt;
            return *this;
        }

        /** @brief --:like i--*/
        TrackNode operator--(int)
        {
            TrackNode tmp = *this;
            pnode = pnode->pre;
            return tmp;
        }

        /** @brief --:like --i*/
        TrackNode& operator --()
        {
            pnode = pnode->pre;
            return *this;
        }

        const TrackNode& operator = (const TrackNode& tracknode) {
            pnode = tracknode.pnode;
            pt3_v = tracknode.pt3_v;
            return *this;
        }

        bool IsNULL() const {
            return !pnode;
        }

        void AddExtra(const double pt3_[3]) {
            pt3 tmp;
            tmp.x = pt3_[0];
            tmp.y = pt3_[1];
            tmp.z = pt3_[2];
            pt3_v.push_back(tmp);
            pnode->extra_idx = pt3_v.size()-1;
            NodePlane::_node* tmpp = pnode;
            while((tmpp = tmpp->nxt) != NULL) {
                tmpp->extra_idx = pnode->extra_idx;
            }
            tmpp = pnode;
            while((tmpp = tmpp->pre) != NULL) {
                tmpp->extra_idx = pnode->extra_idx;
            }
        }

        void AddExtraToNxt(const double pt3_[3]) {
            pt3 tmp;
            tmp.x = pt3_[0];
            tmp.y = pt3_[1];
            tmp.z = pt3_[2];
            pt3_v.push_back(tmp);
            pnode->extra_idx = pt3_v.size()-1;
            NodePlane::_node* tmpp = pnode;
            while((tmpp = tmpp->nxt) != NULL) {
                tmpp->extra_idx = pnode->extra_idx;
            }
        }

        void AddExtraToPre(const double pt3_[3]) {
            pt3 tmp;
            tmp.x = pt3_[0];
            tmp.y = pt3_[1];
            tmp.z = pt3_[2];
            pt3_v.push_back(tmp);
            pnode->extra_idx = pt3_v.size()-1;
            NodePlane::_node* tmpp = pnode;
            while((tmpp = tmpp->pre) != NULL) {
                tmpp->extra_idx = pnode->extra_idx;
            }
        }

        bool HasExtra() const {
            return pnode->extra_idx != -1;
        }

        void GetExtraAsZVectorIndex(long* idx) {
            *idx = pnode->extra_idx;
        }

        long VecIndex() {
            return pnode->extra_idx;
        }

        void GetExtra(double pt3_[3]) const {
            memcpy(pt3_, pt3_v[pnode->extra_idx].v, sizeof(double)*3);
        }
    };
public:
    TrackSpace(const ImgMatchVector& imv, std::vector<float> angles) {}
    TrackSpace():xyplane_array(NULL), info_array(NULL), size(0), nodes_v(NULL) {}
    ~TrackSpace() {
        if(xyplane_array) {
            Release();
        }
    }

    void Release() {
        assert(xyplane_array);
        delete [] xyplane_array;
        delete [] info_array;
        delete [] nodes_v;
        xyplane_array = NULL;
        info_array = NULL;
        nodes_v = NULL;
        pt3_v.clear();
    }

    int Size() const {
        return size;
    }

    float Z_Angle(int z) const {
        return info_array[z].angle;
    }

    void InsertMatchVector(const ImgMatchVector& imv)
	{
		 for(int i = 0; i < imv.Size(); i++){
            NodePlane::_node* pnode1 = NULL, * pnode2 = NULL;
            int idx1 = imv[i].idx1;
            int idx2 = imv[i].idx2;
			
// 	    EX_BEGIN_CLOCK()
            for(int j = 0; j < imv[i].size(); j++) {
                pnode1 = xyplane_array[idx1].AddNode((imv[i].pairs)[j].first);
                pnode2 = xyplane_array[idx2].AddNode((imv[i].pairs)[j].second);
                if(idx1 < idx2) {						//WARNING
                    NodePlane::Link(pnode1, pnode2);
                }
                else if(idx1 > idx2) {
                    NodePlane::Link(pnode2, pnode1);
                }
            }
// 	    EX_END_CLOCK()
// 	    EX_TRACE("\n# Matchs Between MRC[%d] & MRC[%d] Have Been Added(%ld ms)\n", idx1, idx2, EX_ELAPSE())
        }
        
        if(nodes_v){
			delete [] nodes_v;
        }
        
        nodes_v = new std::vector<util::NodePlane::_node*>[size];
        
        for(int i = 0; i < size; i++){
            NodePlane::NodeReader reader = xyplane_array[i].GetReader();
            NodePlane::_node* pnode;
            while((pnode = reader.Next()) != NULL) {
                nodes_v[i].push_back(pnode);
            }
        }
	}
    
    void Create(const ImgMatchVector& imv, const std::vector<float>& angles)
    {
        assert(!xyplane_array);
        EX_TIME_BEGIN("Creating TrackSpace")

        size = angles.size();
        xyplane_array = new util::NodePlane[size];		//create z coordinate
        info_array = new _info[size];

        for(int i = 0; i < size; i++) {
            info_array[i].angle = angles[i];
            info_array[i].z = i;
            xyplane_array[i].SetInfo(&(info_array[i]));
        }
        
        InsertMatchVector(imv);

        EX_TIME_END("Creating TrackSpace")
    }

    Iterator Z_Iterator(int z)
    {
        assert(z >= 0&&z < size);
        Iterator citr(*this);
        citr.pnodes =  &(nodes_v[z]);
        citr.index = 0;
        citr.pt3_v = pt3_v;
        return citr;
    }

    int Z_Size(int z)
    {
        assert(z >= 0&&z < size);
        return nodes_v[z].size();
    }

    int PrintXYPlane(int index, std::ostream& o) const;
	void CoordinateTransform(int width, int height);
	void PreRotate(float angle);		//angle in degree
	static void WriteIMODfidModel(util::TrackSpace& trackspace, int width, int height, int num_frames, const char* filename);
};

static bool ReadAnglesByName(const char* name, std::vector<float>* angles)
{
    std::ifstream in(name);
    if(!in.good()) {
        return false;
    }

    while(in.good()) {
        float angle;
        in>>angle;
        if(in.fail()) {
            break;
        }
        angles->push_back(angle);
    }
    in.close();
    return true;
}

struct trkpt_ {
    int z;
    _point pt;
    trkpt_() {}
    trkpt_(int z_, const _point& pt_) {
        z = z_;
        pt = pt_;
    }
};

class KeyTrack {
public:
    int z;
    _point pt;
    std::vector<trkpt_>	nxtv;
    std::vector<trkpt_>	prev;
};


}
#endif
