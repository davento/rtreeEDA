#include <ctime>
#include <cstdlib>
#include "Rtree.h"
#include "Display.h"

const int kTestsSize = 3;
vector kTests[kTestsSize] = {1,5,20};

class Tester {

    public:
        typedef RtreeNode Node;

        Tester();
        Tester(int n_);

        void insert();
        void search();
        void remove();
        void knn();

    protected:
        friend class Rtree;
        friend class Display;
        friend class Figure;

    private:
        int n;
        int insertBatchSize;
        int searchBatchSize;
        int removeBatchSize;
        int knnBatchSize;
        int polygonLimit;
        clock_t start, end;
        
        double insertBatch(Rtree* &r);
        double searchBatch(Rtree* &r);
        double removeBatch(Rtree* &r, &std::vector<Figure>);
        double knnBatch(Rtree* &r, int k);
};