#include "testing.h"

#include "BN.h"
#include "additional.h"

#include <stdlib.h>
#include <iostream>
#include <inttypes.h>
#include <cstdio>
#include <vector>

using namespace std;

int testingMul_ij(int max1,int max2, int,int) {
    BN bn1(BN::makeRandom(rand() % max1 + 1));
    BN bn2(BN::makeRandom(rand() % max2 + 1));
    BN mul = bn1*bn2;
    BN c_m = bn1.classicMultiplication(bn2);
    BN f_m = bn1.fastMultiplication(bn2);
    BN k_m = bn1.karatsubaMultiplication(bn2);
    BN k_o = bn1.karatsubaMultiplication(bn2);
    if(mul != c_m || mul != f_m || mul != k_m || mul != k_o) {
        printf("bn1:\t");       bn1.PrintDec();
        printf("bn2:\t");       bn2.PrintDec();
        printf("bn1 * bn2:\t");   mul.PrintDec();
        printf("bn1 * bn2 classic:\t"); f_m.PrintDec();
        printf("bn1 * bn2 fast:\t");  f_m.PrintDec();
        printf("bn1 * bn2 karatsuba:\t");  k_m.PrintDec();
        printf("bn1 * bn2 karatsuba_o:\t");  k_o.PrintDec();
        return 1;
    }
    return 0;
}

int testingExp_ij(int max1,int max2, int,int) {
    BN g(BN::makeRandom(rand() % max1 + 1));
    BN exp(BN::makeRandom(rand() % max2 + 1));
    BN mod(BN::makeRandom(rand() % max2 + 1));
    int vsize = rand() % (rand()%2 ? max1 : max2) + 1;
    if(vsize >  (int) sizeof(unsigned int) * 8 - 2)
        return 0;
    if(mod.is0())
        return 0;

    int k_slide = 8;          // TODO - rand!
    int K = 3;
    vector <BN> precomp = g.expLeftToRightK_aryPrecomputation(mod);
    vector <BN> precompVar = g.expLeftToRightK_aryVarPrecomputation(mod, K);
    vector <BN> precompModif = g.expLeftToRightK_aryModifPrecomputation(mod);
    vector <BN> precompSlide = g.expSlidingWindowPrecomputation(mod, k_slide);
    vector <BN> precompSlideU = g.expBest_SlidePrecomp(mod);

    BN res1 = g.expRightToLeft(exp,mod);
    BN res2 = g.expRightToLeft(exp,mod);
    BN res3 = g.expLeftToRightK_ary(exp,mod,precomp);
    BN res3_1 = g.expLeftToRightK_aryVar(exp, mod, precompVar, K);
    BN res4 = g.expLeftToRightK_aryMod(exp,mod,precompModif);
    BN res5 = g.expSlidingWindow(exp, mod, precompSlide, k_slide);

    BN res12 = g.expBest_Slide(exp, mod, precompSlideU);

    if(
            res1 != res2 ||
            res1 != res3 ||
            res1 != res3_1 ||
            res1 != res4 ||
            res1 != res5 ||
            res1 != res12) {
        printf("g:\t");         g.PrintDec();
        printf("exp:\t");       exp.PrintDec();
        printf("mod:\t");       mod.PrintDec();
        printf("bn1->bn2:\t");  res1.PrintDec();
        printf("bn1<-bn2:\t");  res2.PrintDec();
        printf("bn1=>bn2:\t");  res3.PrintDec();
        printf("bn1==bn2:\t");  res3_1.PrintDec();
        printf("bn1!>bn2:\t");  res4.PrintDec();
        printf("bn1>>bn2:\t");  res5.PrintDec();
        printf("bn1##bn2:\t");  res12.PrintDec();
        puts("");
        return 1;
    }
    return 0;
}

int testingMul() {
    for(int i=0;i<=3;i++) {
        int max1=0;
        int max2=0;
        switch(i) {
            case 0:
                max1=3;
                max2=3;
                break;
            case 1:
                max1=100;
                max2=3;
                break;
            case 2:
                max1=3;
                max2=100;
                break;
            case 3:
                max1=100;
                max2=100;
                break;
        }
        for(int j=0;j<100000;j++) {
            int res=testingMul_ij(max1,max2,i,j);
            if(res != 0) {
                printf("Error int Mul-test: i=%d j=%d\ttest=%d\n",i,j,res);
                return res;
            }
        }
    }
    cout<<"Multiple test: OK" << endl;
    return 0;
}

int testingExp() {
    for(int i=0;i<=3;i++) {
        int max1=0;
        int max2=0;
        switch(i) {
            case 0:
                max1=3;
                max2=3;
                break;
            case 1:
                max1=100;
                max2=3;
                break;
            case 2:
                max1=3;
                max2=100;
                break;
            case 3:
                max1=100;
                max2=100;
                break;
        }
        int j_max = 20;
        for(int j=0;j< j_max;j++) {
            int res=testingExp_ij(max1,max2,i,j);
            if(j % 5 == 0)
                cerr << j*100/j_max << "% ...";
            if(res != 0) {
                printf("Error int Exp-test: i=%d j=%d\ttest=%d\n",i,j,res);
                return res;
            }
        }
        cerr<<"100 %" <<endl;
    }
    cerr << "Exp test: OK" << endl;
    return 0;
}

int testing_ij(int max1,int max2, int i,int j)
{
        BN bn1(BN::makeRandom(rand() % max1 + 1));
        BN bn2(BN::makeRandom(rand() % max2 + 1));
        BN mod(BN::makeRandom(rand() % max2 + 1));

    if(
        bn1 != BN::bn0() &&
        bn2 != BN::bn0() &&
        !mod.is0()
    ) {
                if((bn1+bn2-bn1)*bn1!=bn1*bn2)
                {
                        printf("bn1:\t");        bn1.PrintDec();
                        printf("bn2:\t");        bn2.PrintDec();
                        printf("bn1+bn2:\t");        (bn1+bn2).PrintDec();
                        printf("bn1*bn2:\t");        (bn1*bn2).PrintDec();
                        printf("f1:\t");        (bn1+bn2-bn1).PrintDec();
                        printf("f2:\t");        (bn1*bn2/bn1).PrintDec();
                        BN bn1bn2=bn1*bn2;
                        printf("bn1bn2:\t");        bn1bn2.PrintDec();
                        printf("f22:\t");        (bn1bn2/bn1).PrintDec();
                        return 1;
                }
                if((bn1*bn2/bn1/bn2).get64()!=1)
                        return 2;
                if(bn1 - bn1 / bn2 * bn2 != bn1 % bn2 + BN::bn0())
                        return 3;
                if((bn1*bn2).get64()!=bn1.get64()*bn2.get64())
                        return 4;
                if(bn1*bn1!=bn1.Qrt())
                        return 5;
                uint64_t pow=bn1.get64();
                BN powbn(pow);
                if(j % 10 == 0 && mod!= BN::bn0() &&bn2.PowMod(pow,mod)!=bn2.PowMod(powbn,mod))
                {
                        printf("bn2:");bn2.PrintDec();
                        printf("pow:");printf("%" PRIu64 "\n",pow);
                        printf("pBN:");powbn.PrintDec();
                        printf("mod:");mod.PrintDec();
                        printf("Pll:");bn2.PowMod(pow,mod).PrintDec();
                        printf("Pbn:");bn2.PowMod(powbn,mod).PrintDec();
                        return 6;
        }
                BN gcd=gcdEuclidean(bn1,mod);
                if(gcd!=gcdBinary(bn1,mod))
                {
                        gcdEuclidean(bn1,mod).PrintDec();
                        gcdBinary(bn1,mod).PrintDec();
                        bn1.PrintDec();
                        mod.PrintDec();
                        return 7;
                }
                BN gcdInverse=gcdInverseEuclidean(bn1,mod);
                BN gcdInverseBin=gcdInverseEuclideanBinary(bn1,mod);
                if(gcdInverse!=gcdInverseBin)
                {
                        printf("bn1:\t");        bn1.PrintDec();
                        printf("mod:\t");        mod.PrintDec();
                        printf("inverse:\t");        gcdInverse.PrintDec();
                        printf("inverseB:\t");        gcdInverseBin.PrintDec();
                        return 8;
                }
                if(gcd != BN::bn1() || mod == BN::bn1()) {
                        if(gcdInverse!= BN::bn0()) {
                                printf("bn1: ");        bn1.PrintDec();
                                printf("mod: ");        mod.PrintDec();
                                printf("inverse: ");        gcdInverseEuclidean(bn1,mod).PrintDec();
                                printf("nod: ");        gcdBinary(bn1,mod).PrintDec();
                                (gcdInverseEuclidean(bn1,mod)*bn1).PrintDec();
                                (gcdInverseEuclidean(bn1,mod)*bn1%mod).PrintDec();
                                return 9;
                        }
                } else {
                        if(gcdInverse*bn1%mod!= BN::bn1() ||gcdInverse>=mod) {
                                printf("bn1: ");        bn1.PrintDec();
                                printf("mod: ");        mod.PrintDec();
                                printf("inverse: ");        gcdInverseEuclidean(bn1,mod).PrintDec();
                                printf("nod: ");        gcdBinary(bn1,mod).PrintDec();
                                (gcdInverseEuclidean(bn1,mod)*bn1).PrintDec();
                                (gcdInverseEuclidean(bn1,mod)*bn1%mod).PrintDec();
                                return 10;
                        }
                }
                if(i<2) {
                        BN sqrt = bn1.Sqrt();
                        BN sqrt1 = sqrt;
                        ++sqrt1;
                        BN odin(1);
                        if(sqrt*sqrt>bn1||sqrt1*sqrt1<=bn1) {
                                sqrt.PrintDec();
                                bn1.PrintDec();
                                return 11;
                        }
                }
        } else
                if(!mod.is0()) {
                        if(bn1*bn2!= BN::bn0() ||bn1+bn2!=max(bn1,bn2))
                                return 11;
                }
                else if(bn1*mod!= BN::bn0() ||bn1+mod!=max(bn1,mod))
                        return 12;
        return 0;
}

int testingBN()
{
        for(int i=0;i<=3;i++)
        {
                int max1=0;
                int max2=0;
                switch(i) {
                    case 0:
                        max1=3;
                        max2=3;
                        break;
                    case 1:
                        max1=100;
                        max2=3;
                        break;
                    case 2:
                        max1=3;
                        max2=100;
                        break;
                    case 3:
                        max1=100;
                        max2=100;
                        break;
                }
                for(int j=0;j<1000;j++)
                {
                        if(j%250==0)
                                cout<<"i="<<i<<"\tj="<<j<<'\n';
                        int res=testing_ij(max1,max2,i,j);
                        if(res!=0)
                        {
                                printf("Error: i=%d j=%d\ttest=%d\n",i,j,res);
                                return res;
                        }
                }
        }
        return 0;
}

void testing() {
    BN bn1(BN::makeRandom(15000));
    BN bn2(BN::makeRandom(15000));
    std::cout << "Test 1:\t";
    cout.flush();
    if(bn1 + bn2 - bn1 != bn1 * bn2 / bn1) {
        bn1.PrintHex();
        bn2.PrintHex();
        (bn1 + bn2).PrintHex();
        (bn1 + bn2 - bn1).PrintHex();
        (bn1 * bn2).PrintHex();
        (bn1 * bn2 / bn1).PrintHex();
        cout <<" FAIL" << endl;
    } else
        cout << "OK" << endl;

        cout<<"Test 2:\t";
        cout.flush();
        if((bn1*bn2/bn1/bn2).get64()!=1)
                cout<<"FAIL\n";
        else
                cout<<"OK\n";
        cout<<"Test 3:\t";
        cout.flush();
        if(bn1 - bn1 / bn2 * bn2 != bn1 % bn2 + BN::bn0())
                cout<<"FAIL\n";
        else
                cout<<"OK\n";
        cout<<"Test 4:\t";
        cout.flush();
        if((bn1*bn2).get64()!=bn1.get64()*bn2.get64())
                cout<<"FAIL\n";
        else
                cout<<"OK\n";
        cout<<"Test sqrt:\t";
        cout.flush();
        BN sqrt;
        BN bnn(BN::makeRandom(400));
        sqrt=bnn.Sqrt();
        BN sqrt1=sqrt;
        ++sqrt1;
        if(bnn-sqrt*sqrt>bnn||(++sqrt)*(++sqrt)<=bnn)
                cout<<"FAIL\n";
        else
                cout<<"OK\n";
        cout<<"Test qrt:\t";
        cout.flush();
        if(bn1*bn1!=bn1.Qrt())
                cout<<"FAIL\n";
        else
                cout<<"OK\n";
        cout.flush();
        cout<<"Test gcd:\t";
        cout.flush();
        BN bnn1(BN::makeRandom(2500));
        BN bnn2(BN::makeRandom(2500));
        BN bngcd1=gcdEuclidean(bnn1,bnn2);
        cout<<"HaBePHo OK :)"<<endl;
        cout<<"Test binary gcd:\t";
        cout.flush();
        BN bngcd2=gcdBinary(bnn1,bnn2);
        if(bngcd1==bngcd2)
                cout<<"OK\n";
        else
                cout<<"FAIL\n";

        cout.flush();
}

void multest(int base,int test)
{
        uint64_t t;
        vector <BN> v1;
        vector <BN> v2;

        for(int i=0;i<test;i++) {
                v1.push_back(BN::makeRandom(base));
                v2.push_back(BN::makeRandom(base));
        }

        t = clock();
        for(vector <BN> :: iterator i = v1.begin(), j = v2.begin(); i != v1.end(); i++, j++)
            i->classicMultiplication(*j);
        float t1 = clock() - t;

        t = clock();
        for(vector <BN> :: iterator i = v1.begin(), j = v2.begin(); i != v1.end(); i++, j++)
            i->fastMultiplication(*j);
        float t2 = clock() - t;

        t = clock();
        for(vector <BN> :: iterator i = v1.begin(), j = v2.begin(); i != v1.end(); i++, j++)
            i->karatsubaMultiplication(*j);
        float t3 = clock() - t;

        float diviser = test;
        t1 /= diviser;
        t2 /= diviser;
        t3 /= diviser;

        printf("%d\t%d\t%d\t%.2f\t\t%.2f\t\t%.2f\n",base,(int)(base*8),(int)test, t1, t2, t3);
}

void modtest(int base,int test) {
    uint64_t t;
    vector <BN> g;
    vector <BN> exp;
    vector <BN> mod;


    for(int i=0;i<test;i++) {
        g.push_back(BN::makeRandom(base));
        exp.push_back(BN::makeRandom(base));
        BN m(BN::makeRandom(base));
        while (m.is0() || (gcdBinary(m, (BN)bsize) != (BN) 1))
            m = BN::makeRandom(base);
        mod.push_back( m );
    }

    t = clock();
    for(vector <BN> :: iterator i = g.begin(), j = exp.begin(), k = mod.begin(); i != g.end(); i++, j++, k++)
        i->PowMod(*j, *k);
    float t1 = clock() - t;

    t = clock();
    for(vector <BN> :: iterator i = g.begin(), j = exp.begin(), k = mod.begin(); i != g.end(); i++, j++, k++)
        i->PowModBarrett(*j, *k);
    float t2 = clock() - t;

    float diviser = 1000.0 * test;
    t1 /= diviser;
    t2 /= diviser;

    printf("%d\t%d\t%d\t%.2f\t\t%.2f\n",base,(int)(base*8),test, t1, t2);
}

void unitest(int base,int test) {
    uint64_t t;

    BN g(BN::makeRandom(base));
    vector <BN> exp;
    BN mod(BN::makeRandom(base));


    for(int i=0;i<test;i++) {
        exp.push_back(BN::makeRandom(base));
    }

    t = clock();
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expRightToLeft(*e, mod);
    float t1 = clock() - t;

    t = clock();
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expRightToLeft(*e, mod);
    float t2 = clock() - t;

    t = clock();
    vector <BN> precomp = g.expLeftToRightK_aryPrecomputation(mod);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expLeftToRightK_ary(*e, mod, precomp);
    float t3 = clock() - t;

    t = clock();
    vector <BN> precompModif = g.expLeftToRightK_aryModifPrecomputation(mod);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expLeftToRightK_aryMod(*e, mod, precompModif);
    float t4 = clock() - t;

    int k = 8;
    t = clock();
    vector <BN> precompSlide = g.expSlidingWindowPrecomputation(mod, k);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expSlidingWindow(*e, mod, precompSlide, k);
    float t5 = clock() - t;

    float diviser = 1000.0 * test;
    t1 /= diviser;
    t2 /= diviser;
    t3 /= diviser;
    t4 /= diviser;
    t5 /= diviser;

    printf("%d\t%d\t%d\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",base,(int)(base*8),test, t1, t2, t3, t4, t5);
}

void karytest(int K, int base,int test) {
    uint64_t t;

    BN g(BN::makeRandom(base));
    vector <BN> exp;
    BN mod(BN::makeRandom(base));

    for(int i=0;i<test;i++) {
        exp.push_back(BN::makeRandom(base));
    }

    t = clock();
    if(1 || K == 8) {
        vector <BN> precomp = g.expLeftToRightK_aryPrecomputation(mod);
        for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
            g.expLeftToRightK_ary(*e, mod, precomp);
    }
    float t1 = clock() - t;

    t = clock();
    vector <BN> precompVar = g.expLeftToRightK_aryVarPrecomputation(mod, K);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expLeftToRightK_aryVar(*e, mod, precompVar, K);
    float t2 = clock() - t;

    float diviser = 1000.0 * test;
    t1 /= diviser;
    t2 /= diviser;

    printf("%d\t%d\t%d\t%.2f\t\t%.2f\n",base,(int)(base*8),test, t1, t2);
}

void slidetest(int base,int test) {
    uint64_t t;

    BN g(BN::makeRandom(base));
    vector <BN> exp;
    BN mod(BN::makeRandom(base));


    for(int i=0;i<test;i++) {
        exp.push_back(BN::makeRandom(base));
    }

    int k;
    vector <BN> precompSlide;

    k = 4;
    t = clock();
    precompSlide = g.expSlidingWindowPrecomputation(mod, k);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expSlidingWindow(*e, mod, precompSlide, k);
    float t1 = clock() - t;

    k = 8;
    t = clock();
    precompSlide = g.expSlidingWindowPrecomputation(mod, k);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expSlidingWindow(*e, mod, precompSlide, k);
    float t2 = clock() - t;

    k = 9;
    t = clock();
    precompSlide = g.expSlidingWindowPrecomputation(mod, k);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expSlidingWindow(*e, mod, precompSlide, k);
    float t3 = clock() - t;

    k = 10;
    t = clock();
    precompSlide = g.expSlidingWindowPrecomputation(mod, k);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expSlidingWindow(*e, mod, precompSlide, k);
    float t4 = clock() - t;

    k = 11;
    t = clock();
    precompSlide = g.expSlidingWindowPrecomputation(mod, k);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expSlidingWindow(*e, mod, precompSlide, k);
    float t5 = clock() - t;

    float diviser = 1000.0;
    t1 /= diviser;
    t2 /= diviser;
    t3 /= diviser;
    t4 /= diviser;
    t5 /= diviser;

    printf("%d\t%d\t%d\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",base,(int)(base*8),test, t1, t2, t3, t4, t5);
}

void restest(int base,int test) {
    uint64_t t;

    BN g(BN::makeRandom(base));
    vector <BN> exp;
    BN mod(BN::makeRandom(base));

    for(int i=0;i<test;i++) {
        exp.push_back(BN::makeRandom(base));
    }

    t = clock();
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expRightToLeft(*e, mod);
    float t1 = clock() - t;

    t = clock();
    vector <BN> precomp_expBest_Slide = g.expBest_SlidePrecomp(mod);
    for(vector <BN> :: iterator e = exp.begin(); e != exp.end(); e++)
        g.expBest_Slide(*e, mod, precomp_expBest_Slide);
    float t2 = clock() - t;

    float diviser = 1000 * test;
    t1 /= diviser;
    t2 /= diviser;

    printf("%d\t%d\t%d\t%.2f\t\t%.2f\n",base,(int)(base*8),test, t1, t2);
}


void resulttest() {
    cout<<"Test \"multiplication\":"<<endl;
    cout<<"Base\tBit\tTests\tClassic (µs)\tComma's (µs)\tKaratsuba (µs)"<<endl;

    multest(16, 500000);
    multest(32, 100000);
    multest(64, 50000);
    multest(128, 20000);
    multest(256, 2000);
    multest(512, 1000);
    multest(1024, 200);
    multest(2048, 50);
    multest(4096, 20);

    cout<<"Test \"reduction\":"<<endl;
    cout<<"Base\tBit\tTests\tClassic (ms)\tBarrett (ms)"<<endl;

    modtest(16, 50);
    modtest(32, 25);
    modtest(64, 5);
    modtest(128, 5);

    cout<<"Test \"Universal Methods\":"<<endl;
    cout<<"Base\tBit\tTests\tRight-to-Left(ms)\tRight-to-Left (ms)\tk-ary (ms) \t k-ary [mod] (ms)\tsliding (ms)"<<endl;

    unitest(16, 50);
    unitest(32, 25);
    unitest(64, 5);
    unitest(128, 5);

    cout<<"Test \"k-ary method\":"<<endl;
    cout<<"Base\tBit\tTests\tk\tk-ary (ms) \t k-ary [Var] (ms)\t"<<endl;

    karytest(6, 16, 50);
    karytest(6, 32, 25);
    karytest(6, 64, 5);
    karytest(6, 128, 5);

    karytest(8, 16, 50);
    karytest(8, 32, 25);
    karytest(8, 64, 5);
    karytest(8, 128, 5);

    karytest(10, 16, 50);
    karytest(10, 32, 25);
    karytest(10, 64, 5);
    karytest(10, 128, 5);

    cout<<"Test \"k in Sliding Window\":"<<endl;
    cout<<"Base\tBit\tTests\tRight-to-Right (ms)\tRight-to-Left (ms)\tk-ary (ms) \t k-ary [mod] (ms)\tsliding (ms)"<<endl;

    slidetest(16, 50);
    slidetest(32, 25);
    slidetest(64, 5);
    slidetest(128, 5);

    cout<<"Test \"Super-Method\":"<<endl;
    cout<<"Base\tBit\tTests\tUniversal (ms)\tSlide mod(ms)"<<endl;

    restest(16, 50);
    restest(32, 25);
    restest(64, 5);
    restest(128, 5);
}

