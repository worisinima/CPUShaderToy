
#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include<random>
#include<stdlib.h>
#include<time.h>
#include <cmath>

#include <windows.h>
#include <io.h>
#include <wrl.h>
#include <array>
#include <direct.h>
#include "shlobj.h" 

//CImage
#include "CImage/CImg.h"
using namespace cimg_library;
using namespace std;
using namespace Microsoft::WRL;

namespace FileHelper
{
	/*	Example:
		vector<string> paths;
		const string FilePath = "C:\\Users\\yivanli\\Desktop\\DX12Lab\\DX12Lab\\Textures";
		FileHelper::GetFiles(FilePath, paths);
	*/
	static void GetFiles(string path, vector<string>& files)
	{
		//文件句柄  
		long long hFile = 0;
		//文件信息  
		struct _finddata_t fileinfo;
		string p;
		if ((hFile = _findfirst(p.assign(path).c_str(), &fileinfo)) != -1)
		{
			do
			{
				//如果是目录,迭代之  
				//如果不是,加入列表  
				if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						GetFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
				else
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	/*	Example:
		string path;
		FileHelper::GetProjectPath(path);
		假设在E盘的工程，那么这个值会为：
		"E:\\DX12\\DX12Lab\\DX12Lab"
	*/
	static void GetProjectPath(string& OutPath)
	{
		char* buffer;
		//也可以将buffer作为输出参数
		if ((buffer = _getcwd(NULL, 0)) != NULL)
		{
			OutPath = buffer;
		}
	}

	/*	CSIDL_BITBUCKET 回收站
		CSIDL_CONTROLS 控制面板
		CSIDL_DESKTOP Windows桌面desktop;
		CSIDL_DESKTOPDIRECTORY desktop 的目录；
		CSIDL_DRIVES 我的电脑
		CSIDL_FONTS 字体目录
		CSIDL_NETHOOD 网上邻居
		CSIDL_NETWORK 网上邻居virtual folder
		CSIDL_PERSONAL 我的文档
		CSIDL_PRINTERS 打印机
		CSIDL_PROGRAMS 程序组
		CSIDL_RECENT 最近打开文档
		CSIDL_SENDTO 发送到菜单项
		CSIDL_STARTMENU 快启菜单
		CSIDL_STARTUP 启动目录
		CSIDL_TEMPLATES 临时文档
	*/

	static string GetDesktopPath()
	{
		LPITEMIDLIST pidl;
		LPMALLOC pShellMalloc;
		char szDir[1024];
		if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
		{
			if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl))) {
				// 如果成功返回true  
				SHGetPathFromIDListA(pidl, szDir);
				pShellMalloc->Free(pidl);
			}
			pShellMalloc->Release();
		}

		string Ret = string(szDir);
		//Ret.replace(Ret.find_first_of(""), filePath.length(), "")

		return Ret;
	}
}

class FColor
{
public:

	//0~255
	FColor() :R(0), G(0), B(0) {}
	FColor(int r, int g, int b) :R(r), G(g), B(b) {}
	FColor(int val) :R(val), G(val), B(val) {}
	~FColor() {}
	FColor operator+ (const FColor& Ref)
	{
		FColor ret;
		ret.R = this->R + Ref.R;
		ret.G = this->G + Ref.G;
		ret.B = this->B + Ref.B;
		return ret;
	}
	FColor operator/ (const FColor& Ref)
	{
		FColor ret;
		ret.R = this->R / Ref.R;
		ret.G = this->G / Ref.G;
		ret.B = this->B / Ref.B;
		return ret;
	}
	FColor operator/ (const float& fval)
	{
		FColor ret;
		ret.R = this->R / fval;
		ret.G = this->G / fval;
		ret.B = this->B / fval;
		return ret;
	}
	void Print() { cout << "R: " << R << "  " << "G: " << G << "  " << "B: " << B << endl; }
	int R, G, B;
};

class FVector2D
{
public:
	FVector2D(float x, float y) : X(x), Y(y) {}
	FVector2D() :X(0.0f), Y(0.0f) {}
	~FVector2D() {}
	FVector2D operator+ (const FVector2D& Ref) { FVector2D ret; ret.X = this->X + Ref.X; ret.Y = this->Y + Ref.Y; return ret; }
	FVector2D operator- (const FVector2D& Ref) { FVector2D ret; ret.X = this->X - Ref.X; ret.Y = this->Y - Ref.Y; return ret; }
	FVector2D operator* (const FVector2D& Ref) { FVector2D ret; ret.X = this->X * Ref.X; ret.Y = this->Y * Ref.Y; return ret; }
	FVector2D operator* (const float& fval) { FVector2D ret; ret.X = this->X * fval; ret.Y = this->Y * fval; return ret; }
	FVector2D operator/ (const FVector2D& Ref) { FVector2D ret; ret.X = this->X / Ref.X; ret.Y = this->Y / Ref.Y; return ret; }
	FVector2D operator/ (const float& fval) { FVector2D ret; ret.X = this->X / fval; ret.Y = this->Y / fval; return ret; }
	void operator= (const FVector2D& Ref) { this->X = Ref.X; this->Y = Ref.Y; }
	bool operator== (const FVector2D& Ref)const { return this->X == Ref.X && this->Y == Ref.Y; }
	bool operator!= (const FVector2D& Ref)const { return this->X != Ref.X || this->Y != Ref.Y; }
	void Print() { cout << "X: " << X << "  " << "Y: " << Y << endl; }
	float X, Y;
};

class FVector3D
{
public:
	FVector3D(float x, float y, float z, float w) : X(x), Y(y), Z(z) {}
	FVector3D() :X(0.0f), Y(0.0f), Z(0.0f) {}
	~FVector3D() {}
	FVector3D operator+ (const FVector3D& Ref)
	{
		FVector3D ret;
		ret.X = this->X + Ref.X;
		ret.Y = this->Y + Ref.Y;
		ret.Z = this->Z + Ref.Z;
		return ret;
	}
	FVector3D operator- (const FVector3D& Ref)
	{
		FVector3D ret;
		ret.X = this->X - Ref.X;
		ret.Y = this->Y - Ref.Y;
		ret.Z = this->Z - Ref.Z;
		return ret;
	}
	FVector3D operator* (const FVector3D& Ref)
	{
		FVector3D ret;
		ret.X = this->X * Ref.X;
		ret.Y = this->Y * Ref.Y;
		ret.Z = this->Z * Ref.Z;
		return ret;
	}
	FVector3D operator/ (const FVector3D& Ref)
	{
		FVector3D ret;
		ret.X = this->X / Ref.X;
		ret.Y = this->Y / Ref.Y;
		ret.Z = this->Z / Ref.Z;
		return ret;
	}
	FVector3D operator/ (const float& fval)
	{
		FVector3D ret;
		ret.X = this->X / fval;
		ret.Y = this->Y / fval;
		ret.Z = this->Z / fval;
		return ret;
	}
	FVector3D operator= (const FVector3D& Ref)
	{
		this->X = Ref.X;
		this->Y = Ref.Y;
		this->Z = Ref.Z;
	}
	bool operator== (const FVector3D& Ref)
	{
		return
			this->X == Ref.X &&
			this->Y == Ref.Y &&
			this->Z == Ref.Z;
	}
	bool operator!= (const FVector3D& Ref)
	{
		return
			this->X != Ref.X ||
			this->Y != Ref.Y ||
			this->Z != Ref.Z;
	}
	void Print() { cout << "X: " << X << "  " << "Y: " << Y << "  " << "Z: " << Z << endl; }
	float X, Y, Z;
};

class FVector4D
{
public:
	FVector4D(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
	FVector4D() :X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
	~FVector4D() {}
	FVector4D operator+ (const FVector4D& Ref)
	{
		FVector4D ret;
		ret.X = this->X + Ref.X;
		ret.Y = this->Y + Ref.Y;
		ret.Z = this->Z + Ref.Z;
		ret.W = this->W + Ref.W;
		return ret;
	}
	FVector4D operator- (const FVector4D& Ref)
	{
		FVector4D ret;
		ret.X = this->X - Ref.X;
		ret.Y = this->Y - Ref.Y;
		ret.Z = this->Z - Ref.Z;
		ret.W = this->W - Ref.W;
		return ret;
	}
	FVector4D operator* (const FVector4D& Ref)
	{
		FVector4D ret;
		ret.X = this->X * Ref.X;
		ret.Y = this->Y * Ref.Y;
		ret.Z = this->Z * Ref.Z;
		ret.W = this->W * Ref.W;
		return ret;
	}
	FVector4D operator/ (const FVector4D& Ref)
	{
		FVector4D ret;
		ret.X = this->X / Ref.X;
		ret.Y = this->Y / Ref.Y;
		ret.Z = this->Z / Ref.Z;
		ret.W = this->W / Ref.W;
		return ret;
	}
	FVector4D operator/ (const float& fval)
	{
		FVector4D ret;
		ret.X = this->X / fval;
		ret.Y = this->Y / fval;
		ret.Z = this->Z / fval;
		ret.W = this->W / fval;
		return ret;
	}
	FVector4D operator= (const FVector4D& Ref)
	{
		this->X = Ref.X;
		this->Y = Ref.Y;
		this->Z = Ref.Z;
		this->W = Ref.W;
	}
	bool operator== (const FVector4D& Ref)
	{
		return
			this->X == Ref.X &&
			this->Y == Ref.Y &&
			this->Z == Ref.Z &&
			this->W == Ref.W;
	}
	bool operator!= (const FVector4D& Ref)
	{
		return
			this->X != Ref.X ||
			this->Y != Ref.Y ||
			this->Z != Ref.Z ||
			this->W != Ref.W;
	}
	void Print() { cout << "X: " << X << "  " << "Y: " << Y << "  " << "Z: " << Z << "  " << "W: " << W << endl; }
	float X, Y, Z, W;
};

class FImage
{
public:

	//PPM Image
	FImage(int X, int Y, string ImageName) :
		SizeX(X),
		SizeY(Y),
		ImageName(ImageName)
	{
		for (int i = 0; i < SizeX; i++)
		{
			vector<FColor> row(SizeY);
			Imagedata.push_back(row);
		}
	}

	bool SaveImageToDesk()
	{
		const string& Path = FileHelper::GetDesktopPath();

		cout << endl << "begin save image to desk operation" << endl;

		bool bCreateImage = false;

		ofstream fout(Path + "/" + ImageName + ".ppm");
		fout << "P3\n" << SizeX << " " << SizeY << "\n255\n";

		for (int y = 0; y < SizeY; y++)
		{
			for (int x = 0; x < SizeX; x++)
			{
				FColor& color = Imagedata[x][y];
				fout << color.R << " " << color.G << " " << color.B << "\n";
			}
		}
		fout.close();

		cout << endl << "Save image successfully" << endl;

		return bCreateImage;
	}

	void ClearImage(FColor& ClearColor)
	{
		cout << endl << "Begin clear image operation" << endl;

		int nx = SizeX;
		int ny = SizeY;

		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				Imagedata[i][j] = ClearColor;
			}
		}
	}

	void SetPixleColor(const FColor& newData, const FVector2D& PixleLocation)
	{
		if (PixleLocation.X < SizeX && PixleLocation.Y < SizeY)
			Imagedata[PixleLocation.X][PixleLocation.Y] = newData;
	}
	void SetPixleColor(const int& newData, const FVector2D& PixleLocation)
	{
		if (PixleLocation.X < SizeX && PixleLocation.Y < SizeY)
			Imagedata[PixleLocation.X][PixleLocation.Y] = FColor(newData);
	}

	int SizeX;
	int SizeY;
	string ImageName;
	string ImagePath;
	vector<vector<FColor>> Imagedata;
};

float RandInRange(const int& min, const int& max)
{
	return (rand() % (max - min + 1)) + min;
}

bool InRange(const float& val, const float& min, const float& max, bool includeBoarder = false)
{
	if (includeBoarder) { if (val >= min && val <= max) return true; else return false; }
	else { if (val > min && val < max) return true; else return false; }
}

