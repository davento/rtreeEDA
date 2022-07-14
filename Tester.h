#include <ctime>
#include <cstdlib>
#include "Rtree.h"
#include "Display.h"

const int kTestsSize = 3;
int kTests[kTestsSize] = {1,5,20};

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
        
        double insertBatch(Rtree* &);
        double searchBatch(Rtree* &);
        double removeBatch(Rtree* &, std::vector<Figure> &);
        double knnBatch(Rtree* &, int);
};