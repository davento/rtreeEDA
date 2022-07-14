#include "Tester.h"

static const int kTestsSize = 3;
int kTests[kTestsSize] = {1,5,20};

Tester::Tester() : Tester(100, 800){ }

Tester::Tester(int n_, int l_) : n(n_), dim(l_) {
    insertBatchSize = 10;
    searchBatchSize = 1000;
    removeBatchSize = 1000;
    knnBatchSize = 1000;
    polygonLimit = 5000;
}

void Tester::insert() {
    Rtree * r = new Rtree();
    std::vector<double> exp_time(polygonLimit/insertBatchSize, 0) ;
    for(int e = 0; e < n; e++) {
        std::vector<double> times;
        for(int i = 0; i < polygonLimit/insertBatchSize; i++) {
            times.push_back(insertBatch(r));
        }
        
        for(int i = 0; i < polygonLimit/insertBatchSize; i++) {
            exp_time[i] += times[i];
        }

        delete r;
        r = new Rtree();
    }
    int i = 1;
    for(auto it: exp_time){
        printf("t%d,%f\n",i++, it/ n);
    }

}

void Tester::search() {
    Rtree * r = new Rtree();
    

    std::vector<double> exp_time(polygonLimit/searchBatchSize, 0) ;

    for(int e = 0; e < n; e++) {
        std::vector<double> times;
        for(int i = 0; i < polygonLimit/searchBatchSize; i++) {
            times.push_back(searchBatch(r));
        }

        for(int i = 0; i < polygonLimit/searchBatchSize; i++) {
            exp_time[i] += times[i];
        }

        delete r;
        r = new Rtree();
    }

    int i = 1;
    for(auto it: exp_time){
        printf("t%d,%f\n",i++, it/ n);
    }
}

void Tester::remove(){

    Rtree * r = new Rtree();
    std::vector<double> exp_time(polygonLimit/searchBatchSize, 0) ;

    for(int e = 0; e < n; e++) {
        std::vector<double> times;
        std::vector<Figure> figs;
        for(int i = 0; i < polygonLimit; i++) {
            Figure f = Display::generateRandomFigure(dim);
            figs.push_back(f);
        }
        
        for(int i = 0; i < polygonLimit/removeBatchSize; i++) {
            times.push_back(removeBatch(r, figs));
        }
        double avg = 0;
        for(auto t : times) {
            avg += t;
        }
        delete r;
        r = new Rtree();
    }
}

void Tester::knn() {
    Rtree * r = new Rtree();
    for(int k = 0; k < kTestsSize; k++) {
        for(int e = 0; e < n; e++) {
            std::vector<double> times;
            for(int i = 0; i < polygonLimit/knnBatchSize; i++) {
                times.push_back(knnBatch(r, kTests[i]));
            }
            double avg = 0;
            for(auto t : times) {
                avg += t;
            }
            printf("Tiempo promedio del experimento: %f ms", avg/(polygonLimit/searchBatchSize));
        }
    }
}

double Tester::insertBatch(Rtree* &r) {
            std::vector<Figure> figs;
            for(int i = 0; i < insertBatchSize; i++) {
                auto f = Display::generateRandomFigure(dim);
                figs.push_back(f);
            }
            start = std::clock();
            for(auto f : figs) {
                r->insert(f);
            }
            end = std::clock();
            // printf("Tiempo total: %f ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
            return ((double)(end-start))*1000/CLOCKS_PER_SEC;
        }

double Tester::searchBatch(Rtree* &r) {
        std::vector<Figure> figs;
    for(int i = 0; i < searchBatchSize; i++) {
        Figure f = Display::generateRandomFigure(dim);
        figs.push_back(f);
        r->insert(f);
    }
    start = std::clock();
    for(auto f : figs) {
        r->search(f.getCentroidPoint());
    }
    end = std::clock();
    // printf("Tiempo total: %f ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
    return ((double)(end-start))*1000/CLOCKS_PER_SEC;
}

double Tester::removeBatch(Rtree* &r, std::vector<Figure> &figs) {
    start = std::clock();
    for(auto f : figs) {
        r->remove(f.getCentroidPoint());
    }
    end = std::clock();
    printf("Tiempo total: %f ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
    return ((double)(end-start))*1000/CLOCKS_PER_SEC;
}

double Tester::knnBatch(Rtree* &r, int k) {
    std::vector<Figure> figs;
    for(int i = 0; i < searchBatchSize; i++) {
        Figure f = Display::generateRandomFigure(dim);
        figs.push_back(f);
        r->insert(f);
    }
    start = std::clock();
    for(auto f : figs) {
        r->depthFirst(f.getCentroidPoint(), k);
    }
    end = std::clock();
    printf("Tiempo total: %f ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
    return ((double)(end-start))*1000/CLOCKS_PER_SEC;
}