#ifndef __C2_LSH_H__
#define __C2_LSH_H__

#include <stdint.h>
#include <string>
#include <string.h>
#include <iostream>


using namespace std;

namespace caravel {

	class C2Lsh
	{
	public:
		C2Lsh();
		~C2Lsh();

        static double ComputeL2(double *ardX, double *ardY, uint32_t uiD);

		void Init(uint32_t uiD, uint32_t uiL, double dW);

		void Compute(double *arMatrix, uint32_t *aruiRet);

	private:

        uint32_t m_uiD;
        uint32_t m_uiL;
        double m_dW;

		double genGaussianRandom();

		uint32_t genUint32Random(uint32_t uiRangeStart, uint32_t uiRangeEnd);

		double genUniformRandom(double dRangeStart, double dRangeEnd);

        void getMaxMin(double *arVal, uint32_t uiNum, double &dMax, double &dMin);

		/*
         *   -->    -->
		 *   arA  x  V  + dB   
		 * -------------------
		 *  [       W       ]
		 */

		typedef struct _stLshFunction
		{
			double dB;
			double *arA;
		}stLshFunction, *PLSHFUNC;

        PLSHFUNC *m_parLshFunction;

	};


}

#endif
