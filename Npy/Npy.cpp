// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 - 2024, the Anboto author and contributors
#include <Core/Core.h>
#include <Functions4U/Functions4U.h>

#include "Npy.h"

namespace Upp {

bool Npz::Load(const char *fileName) {
	if (!unzip.Create(fileName))
		return false;
	data.Clear();
	while(!(unzip.IsEof() || unzip.IsError())) {
		if (GetFileExt(unzip.GetPath()) == ".npy") {
			Npy &npy = data.Add();
			npy.name = unzip.GetPath();
			unzip.SkipFile();
		}
	}
	return true;
}

bool Npz::Save(const char *fileName) {
	FileZip zip(fileName);
	
	for (Npy &npy : data) {
		if (npy.data.IsEmpty()) 
			npy.data = unzip.ReadFile(npy.name.begin());
		if (!npy.data.IsEmpty()) {
			String d;
			npy.SaveData(d);
			zip.WriteFile(d, npy.name, Null, Null, false);		
		}
	}
	zip.Finish();
	
	return true;
}

int Npz::size() {
	return data.size();
}

UVector<String> Npz::GetList() {
	UVector<String> ret;
	
	for (const Npy &npz : data)
		ret << npz.name;
	
	return ret;
}

Npy &Npz::Get(const char *name) {
	String sname = ForceExt(name, ".npy");
	static Npy nil;
	for (Npy &npy : data) {
		if (npy.name == sname) {
			if (npy.data.IsEmpty())
				npy.LoadData(unzip.ReadFile(npy.name.begin()));
			if (!npy.data.IsEmpty())
				return npy;
			else
				return nil;
		}
	}
	return nil;
}

Npy &Npz::Add(const char *name) {
	String sname = ForceExt(name, ".npy");
	Npy &ret = data.Add();
	ret.name = sname;
	return ret;
}

void Npz::Remove(const char *name) {
	String sname = ForceExt(name, ".npy");
	for (int i = 0; i < data.size(); ++i) {
		if (data[i].name == sname) {
			data.Remove(i);
			return;
		}
	}
}
	
bool Npy::Load(const char *fileName) {
	LoadData(LoadFile(fileName));
	
	name = fileName;
	return true;
}

void Npy::LoadData(const String &dat) {
	std::istringstream iss(dat.ToStd(), std::ifstream::binary);
	if (!iss)
    	throw std::runtime_error("io error: failed to open a file.");
	std::string header_s = npy::read_header(iss);
	header = npy::parse_header(header_s);
	int size = npy::comp_size(header.shape)*header.dtype.itemsize;
	StringBuffer str(size);
	iss.read(str.begin(), size);
	data = str;
}

bool Npy::Save(const char *fileName) const {
	String d;
	SaveData(d);
	return SaveFile(fileName, d);
}

void Npy::SaveData(String &dat) const {
	std::ostringstream oss(std::ios::binary);
	write_header(oss, header);		
	oss.write(reinterpret_cast<const char *>(data.begin()), data.GetCount());	
	dat = oss.str();
}

String Npy::Type() {
	String ret;
	switch (header.dtype.kind) {
	case 'u': ret = "unsigned"; break;	
	case 'f': ret = "float"; 	break;	
	case 'i': ret = "int"; 		break;	
	case 'c': ret = "complex"; 	break;
	default:  ret = Format("unknown(%c)", header.dtype.kind); 	break;
	}
	ret << header.dtype.itemsize;
	return ret;
}

}