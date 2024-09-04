#ifndef _Npy_Npy_h_
#define _Npy_Npy_h_

#include <Core/Core.h>
#include <Eigen/Eigen.h>
#include <Eigen/MultiDimMatrix.h>
#include <plugin/zip/zip.h>
#include <plugin/libnpy/npy.hpp>

namespace Upp {
	
class Npy {
public:
	bool Load(const char *fileName);
	void LoadData(const String &data);
	bool Save(const char *fileName) const;
	void SaveData(String &dat) const;
	
	template <typename Scalar> 
	bool IsType() const {
		const npy::dtype_t dtype = npy::dtype_map.at(std::type_index(typeid(Scalar)));
		return header.dtype.tie() == dtype.tie();
	}
	String Type();
	
	bool IsNull()	{return data.IsEmpty();}
	
	Vector<int> GetSize() {
		Vector<int> ret;
		for (auto r : header.shape)
			ret << r;
		return ret;	
	}
	
	template <typename Scalar>
	void Get(Eigen::Matrix<Scalar, Eigen::Dynamic, 1> &ret) {
		if (!IsType<Scalar>())
			throw Exc("Data type doesn't match");
		
		int size = npy::comp_size(header.shape);
  		ret.resize(size);
  		memcpy(ret.data(), data.begin(), data.GetCount());
	}
	template <typename Scalar>
	void Get(Upp::Vector<Scalar> &ret) {
		if (!IsType<Scalar>())
			throw Exc("Data type doesn't match");
		
		int size = npy::comp_size(header.shape);		
  		ret.SetCount(size);
		memcpy(ret.begin(), data.begin(), data.GetCount());
	}
	template <typename Scalar>
	void Get(std::vector<Scalar> &ret) {
		if (!IsType<Scalar>())
			throw Exc("Data type doesn't match");
		
		int size = npy::comp_size(header.shape);		
  		ret.resize(size);
		memcpy(ret.begin(), data.begin(), data.GetCount());
	}
	template <typename Scalar>
	void Get(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &ret) {
		if (!IsType<Scalar>())
			throw Exc("Data type doesn't match");
		
		if (header.shape.size() != 2)
			throw Exc("Data dimensions doesn't match");
		
		Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rowmajor;
		rowmajor.resize(header.shape[0], header.shape[1]);
		memcpy(rowmajor.begin(), data.begin(), data.GetCount());
		ret = rowmajor;
	}
	template <typename Scalar>
	void Get(MultiDimMatrixRowMajor<Scalar> &ret) {
		if (!IsType<Scalar>())
			throw Exc("Data type doesn't match");
		
		Vector<int> dim(header.shape.size());
		for (int i = 0; i < dim.size(); ++i)
			dim[i] = header.shape[i];
		ret.Resize(dim);		
		memcpy(ret.begin(), data.begin(), data.GetCount());
	}
	
	template <typename Scalar>
	void Set(const Eigen::Matrix<Scalar, Eigen::Dynamic, 1> &d) {
		header.dtype = npy::dtype_map.at(std::type_index(typeid(Scalar)));;
  		header.fortran_order = false;
  		header.shape = {(npy::ndarray_len_t)d.size()};

		int size = d.size()*header.dtype.itemsize;
		std::ostringstream oss(std::ios::binary);
		if (!oss)
    		throw std::runtime_error("io error: failed to open a block");
		oss.write(reinterpret_cast<const char *>(d.begin()), size);	
		data = oss.str();	
	}
	template <typename Scalar>
	void Set(const Upp::Vector<Scalar> &d) {
		header.dtype = npy::dtype_map.at(std::type_index(typeid(Scalar)));;
  		header.fortran_order = false;
  		header.shape = {(npy::ndarray_len_t)d.size()};

		int size = d.size()*header.dtype.itemsize;
		std::ostringstream oss(std::ios::binary);
		if (!oss)
    		throw std::runtime_error("io error: failed to open a block");
		oss.write(reinterpret_cast<const char *>(d.begin()), size);	
		data = oss.str();
	}
	template <typename Scalar>
	void Set(const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &d) {
		header.dtype = npy::dtype_map.at(std::type_index(typeid(Scalar)));;
  		header.fortran_order = false;
  		
  		header.shape.clear();
  		for (int i = 0; i < d.dims(); ++i)
  			header.shape.push_back(d.size(i));	

		int size = npy::comp_size(header.shape)*header.dtype.itemsize;
		std::ostringstream oss(std::ios::binary);
		if (!oss)
    		throw std::runtime_error("io error: failed to open a block");
		
		Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> rowmajor = d;
		
		oss.write(reinterpret_cast<const char *>(rowmajor.begin()), size);	
		data = oss.str();	
	}
	template <typename Scalar>
	void Set(const MultiDimMatrixRowMajor<Scalar> &d) {
		header.dtype = npy::dtype_map.at(std::type_index(typeid(Scalar)));;
  		header.fortran_order = false;
  		
  		header.shape.clear();
  		for (int i = 0; i < d.dims(); ++i)
  			header.shape.push_back(d.size(i));	

		int size = npy::comp_size(header.shape)*header.dtype.itemsize;
		std::ostringstream oss(std::ios::binary);
		if (!oss)
    		throw std::runtime_error("io error: failed to open a block");
		oss.write(reinterpret_cast<const char *>(d.begin()), size);	
		data = oss.str();
	}
	
	friend class Npz;
	
private:
	String name;
	npy::header_t header;
	String data;
};

class Npz {
public:
	bool Load(const char *fileName);
	bool Save(const char *fileName);
	
	UVector<String> GetList();
	int size();
	
	Npy &Get(const char *name);
	Npy &Add(const char *name);
	void Remove(const char *name);
	
private:
	UArray<Npy> data;
	FileUnZip unzip;
};

}

#endif
