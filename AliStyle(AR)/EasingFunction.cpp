#include "EasingFunction.h"

void binomialCoeffs(int n, int * C){
	int k, j;
	for(k = 0; k <=n; k++){
		C[k] = 1;
		for(j = n; j >= k+1; j--)
			C[k] *= j;
		for(j = n - k ; j >= 2; j--)
			C[k] /= j;
	}
}

void computeBezPt(float u, Point * bezPt, int nCtrlPts, std::vector<Point> ctrlPts, int * C){
	int k , n = nCtrlPts -1;
	float bezBlendFcn;

	bezPt->x = bezPt->y = bezPt->z = 0.0;
	for(k = 0; k < nCtrlPts; k++){
		bezBlendFcn = C[k] * pow(u,k)*pow(1-u,n-k);
		bezPt->x+=ctrlPts[k].x*bezBlendFcn;
		bezPt->y+=ctrlPts[k].y*bezBlendFcn;
		bezPt->z+=ctrlPts[k].z*bezBlendFcn;
	}
}

std::vector<Point> EasingFunction::getBezierPoints(std::vector<Point> controlPts, int nBezCurvePts){
	std::vector<Point> pts;
	float u;
	int * C,k;

	C= new int[controlPts.size()];
	binomialCoeffs(controlPts.size()-1,C);
	for(k = 0 ; k <= nBezCurvePts; k++){
		Point pt;
		u = float(k) / float(nBezCurvePts);
		computeBezPt(u,&pt,controlPts.size(),controlPts,C);
		pts.push_back(pt);
	}
	delete[] C;
	return pts;
}
