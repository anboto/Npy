// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 - 2022, the Anboto author and contributors
#include <Core/Core.h>
#include <Functions4U/Functions4U.h>

using namespace Upp;

#include <Npy/Npy.h>


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	try {
		Npz file;
		
		{
			UppLog() << "\nLoading matrices.npz";
			file.Load(GetDataFile("matrices.npz"));		
	
			UppLog() << "\nMatrices found are: " << file.GetList();
			
			{
				Npy &npdata = file.Get("matrix1");
				if (npdata.IsNull())
					throw Exc("Error data not found");	
				UppLog() << "\nmatrix1: " << npdata.Type();
				
				MultiDimMatrixRowMajor<int> data;
				npdata.Get(data);			
				UppLog() << "\n" << data;
			}
			{
				Npy &npdata = file.Get("matrix2");
				if (npdata.IsNull())
					throw Exc("Error data not found");	
				UppLog() << "\nmatrix2: " << npdata.Type();
				
				MultiDimMatrixRowMajor<double> data;
				npdata.Get(data);			
				UppLog() << "\n" << data;
			}
			
			Npy &nw = file.Add("matrix3");
			UVector<double> vec = {1, 2, 3};
			nw.Set(vec);
			
			file.Save(GetDataFile("matrices_new.npz"));
		}
		{
			UppLog() << "\nLoading matrices_new.npz";
			file.Load(GetDataFile("matrices_new.npz"));		
	
			UppLog() << "\nMatrices found are: " << file.GetList();
			
			{
				Npy &npdata = file.Get("matrix1");
				if (npdata.IsNull())
					throw Exc("Error data not found");	
				UppLog() << "\nmatrix1: " << npdata.Type();
				
				MultiDimMatrixRowMajor<int> data;
				npdata.Get(data);			
				UppLog() << "\n" << data;
			}
			{
				Npy &npdata = file.Get("matrix2");
				if (npdata.IsNull())
					throw Exc("Error data not found");	
				UppLog() << "\nmatrix2: " << npdata.Type();
				
				MultiDimMatrixRowMajor<double> data;
				npdata.Get(data);			
				UppLog() << "\n" << data;
			}
			{
				Npy &npdata = file.Get("matrix3");
				if (npdata.IsNull())
					throw Exc("Error data not found");	
				UppLog() << "\nmatrix3: " << npdata.Type();
				
				MultiDimMatrixRowMajor<double> data;
				npdata.Get(data);			
				UppLog() << "\n" << data;
				
				VERIFY(data(2) == 3.);
			}
		}
		
	} catch(std::runtime_error e) {
		String error = e.what();
		UppLog() << "\n" << error;
		SetExitCode(-1);	
	} catch(Exc e) {
		String error = e;
		UppLog() << "\n" << error;
		SetExitCode(-1);
	} catch(...) {
		UppLog() << "\nError";
		SetExitCode(-1);
	}
			
	UppLog() << "\nProgram ended\n";
	#ifdef flagDEBUG
	ReadStdIn();
	#endif
}

