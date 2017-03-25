#pragma once
#include "stdafx.h"
using namespace cv;
namespace Funs
{
	

	void Salt(double p, const Mat &source, Mat *result = nullptr)
	{
		if (result == nullptr) result = (Mat *)&source;
		else
		{
			source.copyTo(*result);
		}

		//����
		if (p < 0 || p>1) p = 0.1;
		int sum = (int)(p*source.rows*source.cols);

		for (int i = 0; i < sum; ++i)
		{
			int r = rand() % source.rows;
			int c = rand() % source.cols;
			if (source.channels() == 1)
			{
				uchar wspix = 255;
				uchar bspix = 0;
				result->at<uchar>(r, c) = rand() % 2 == 0 ? wspix : bspix;
			}
			else if (source.channels() == 3)
			{
				Vec3b wspix(0xff, 0xff, 0xff);
				Vec3b bspix(0, 0, 0);

				result->at<Vec3b>(r, c) = rand() % 2 == 0 ? wspix : bspix;
			}

		}
		
	}
	///w1 w2 w3��Χ 0-1.0/3
	void Grey(const Mat &source, Mat &result, double w1 = 1.0 / 3, double w2 = 1.0 / 3, double w3 = 1.0 / 3)
	{
		result.create(source.rows, source.cols, CV_8U);
		//Mat_��ʽ����
		Mat_<Vec3b> sitr = source;
		Mat_<uchar> ritr = result;
		for (int r = 0; r < source.rows; r++)
		{
			for (int c = 0; c < source.cols; c++)
			{
				Vec3b pix = sitr(r, c);
				uchar rpix = pix[0] * w1 + pix[1] * w2 + pix[2] * w3;
				ritr(r, c) = rpix;
			}
		}
	}
	///w1 w2 w3��Χ0-1
	void ChangeRGB(const Mat &source, Mat &result, double w1 = 1, double w2 = 1, double w3 = 1)
	{
		result.create(source.rows, source.cols, source.type());
		//��ָ�뷽ʽ����
		for (int r = 0; r < source.rows; ++r)
		{
			const Vec3b *itr = source.ptr<Vec3b>(r);
			Vec3b *ritr = result.ptr<Vec3b>(r);
			for (int c = 0; c < source.cols; ++c)
			{
				const Vec3b &spix = *itr;
				Vec3b &rpix = *ritr;
				rpix[0] = spix[0] * w1;
				rpix[1] = spix[1] * w2;
				rpix[2] = spix[2] * w3;
				itr++;
				ritr++;
			}
		}

	}
	///ͼƬȨ�ص���
	///mask ���ӵ�source1�� ����λ�� mr,mc rgb����Ȩ�� w1 w2 w3
	///Ȩ�� w 0-1
	void user_Add(const Mat &source,const Mat &mask,Mat &result,int mr,int mc,double w1=1,double w2=1,double w3=1)
	{
		
		source.copyTo(result);
		Mat_<Vec3b> ritr = result;
		Mat_<Vec3b> mitr = mask;
		//����ԭȨ��
		double sw1 = 1 - w1;
		double sw2 = 1 - w2;
		double sw3 = 1 - w3;
		for (int r = 0; r < mask.rows&&r+mr<result.rows; ++r)
		{
			for (int c = 0; c < mask.cols&&c+mc<result.cols; ++c)
			{
				Vec3b pix = ritr(r+mr, c+mc);
				pix[0] *= sw1;
				pix[1] *= sw2;
				pix[2] *= sw3;
				//
				Vec3b mpix=mitr(r, c);
				pix[0] += mpix[0] * w1;
				pix[1] += mpix[1] * w2;
				pix[2] += mpix[2] * w3;
				ritr(r + mr, c + mc) = pix;
			}
		}
	}
	///�����˳��� rhΪ���ؿ��� chΪ���ؿ鳤��
	void Mask(const Mat &source, Mat &result,int rh,int ch)
	{
		result.create(source.rows, source.cols, source.type());
		int rsum = source.rows / rh;
		int csum = source.cols / ch;
		Mat_<Vec3b> sitr=source;
		Mat_<Vec3b> ritr=result;
		for (int r = 0; r < rsum; ++r)
		{
			for (int c = 0; c < csum; ++c)
			{
				//r*rh->(r+1)*rhΪ������ c*ch->(c+1)*chΪ������ (rh(2r+1))/2 (rh(2r+1))/2 Ϊ����������ɫ
				//��ȡ��������
				int midr = (rh*(2 * r + 1)) / 2;
				int midc = (ch*(2 * c + 1)) / 2;
				Vec3b mpix = sitr(midr, midc);
				for (int i = r*rh; i < (r + 1)*rh; ++i)
				{
					for (int j = c*ch; j < (c + 1)*ch; ++j)
					{
						if(i<source.rows&&j<source.cols)
							ritr(i, j) = mpix;
					}
				}
			}
		}
	}
	///��Ȩ��ֵ���� ������
	void MaskEX(const Mat &source, Mat &result, int rh, int ch)
	{
		result.create(source.rows, source.cols, source.type());
		int rsum = source.rows / rh;
		int csum = source.cols / ch;
		Mat_<Vec3b> sitr = source;
		Mat_<Vec3b> ritr = result;
		//ƽ������Ȩ��
		double w = 1 / rh*ch;
		//
		for (int r = 0; r < rsum; ++r)
		{
			for (int c = 0; c < csum; ++c)
			{
				//r*rh->(r+1)*rhΪ������ c*ch->(c+1)*chΪ������ (rh(2r+1))/2 (rh(2r+1))/2 Ϊ����������ɫ
				//��ȡ��������
				int midr = (rh*(2 * r + 1)) / 2;
				int midc = (ch*(2 * c + 1)) / 2;
				Vec3b mpix;
				for (int i = r*rh; i < (r + 1)*rh; ++i)
				{
					for (int j = c*ch; j < (c + 1)*ch; ++j)
					{
						if (i < source.rows&&j < source.cols)
						{
							Vec3b pix = sitr(i, j);
							mpix += pix*w;//��һ��ֱ��ʹ����������
						}
					}
				}
				for (int i = r*rh; i < (r + 1)*rh; ++i)
				{
					for (int j = c*ch; j < (c + 1)*ch; ++j)
					{
						if (i<source.rows&&j<source.cols)
							ritr(i, j) = mpix;
					}
				}
			}
		}
	}
	//��ֵ��ȥ������
#define min(a,b) a<b? a:b
#define max(a,b) a>b? a:b
	void ReduceMask(const Mat &source, Mat &result, int rh, int ch)
	{
		result.create(source.rows, source.cols, source.type());
		memset(result.data, 0, result.rows*result.cols*result.elemSize());
		int rsum = source.rows / rh;
		int csum = source.cols / ch;
		Mat_<Vec3b> sitr = source;
		Mat_<Vec3b> ritr = result;
		for (int r = 0; r < rsum; ++r)
		{
			for (int c = 0; c < csum; ++c)
			{
				//r*rh->(r+1)*rhΪ������ c*ch->(c+1)*chΪ������ (rh(2r+1))/2 (rh(2r+1))/2 Ϊ����������ɫ
				//��ȡ��������
				int midr = (rh*(2 * r + 1)) / 2;
				int midc = (ch*(2 * c + 1)) / 2;
				Vec3b mpix = sitr(midr, midc); //��ֵ���� 
				//������������
				//����Ȩ
				double w = 1.0 / 2;
				Vec2i neararr[4] = { {midr - rh,midc},{midr + rh,midc},{midr,midc - ch},{midr,midc+ch} };
				for (int t = 0; t < 4; ++t)
				{
					Vec2i pos = neararr[t]; //Ŀ������
					//�Ϸ��Լ��
					if (pos[0] < 0 || pos[1] < 0 || pos[0] >= source.rows || pos[1] >= source.cols)
					{
						continue;//�������
					}
					//�Ϸ�
					Vec2i fpos = { midr,midc }; //�õ�Դ����
					int m = norm(pos - fpos);//������֮��ľ���
					//�Ϸ� ȡĿ����ֵ
					Vec3b dmpix = sitr(pos[0], pos[1]);
					//���������ر仯�� ����Ȩw RGB��ɫ�ռ�
					double d1 = (double)(dmpix[0]-mpix[0])/m;
					double d2 = (double)(dmpix[1] - mpix[1]) / m;
					double d3 = (double)(dmpix[2] - mpix[2])/m;
					//
					//������ѭ������ѭ���������յ�
					//��ѭ��������� ��ѭ����λ
					Vec2i wvec; //��ѭ������յ�
					Vec2i nvec;//��ѭ������յ�
					int step;
					switch (t)
					{
					case 0:
						wvec[0] = fpos[0];
						wvec[1] = pos[0];
						nvec[0] = c*ch;
						nvec[1] = (c+1)*ch;
						step = -1;
						break;
					case 1:
						wvec[0] = fpos[0];
						wvec[1] = pos[0];
						nvec[0] = c*ch;
						nvec[1] = (c + 1)*ch;
						step = 1;
						break;
					case 2:
						wvec[0] = fpos[1];
						wvec[1] = pos[1];
						nvec[0] = r*rh;
						nvec[1] = (r + 1)*rh;
						step = -1;
						break;
					case 3:
						wvec[0] = fpos[1];
						wvec[1] = pos[1];
						nvec[0] = r*rh;
						nvec[1] = (r + 1)*rh;
						step = 1;
						break;
					}
					for (int i = wvec[0],tm=0; i < wvec[1]; ++i,tm++)
					{
						for (int j = nvec[0]; j < nvec[1]; ++j)
						{
							Vec3b spix = { (uchar)(d1*tm + mpix[0]),(uchar)(d2*tm + mpix[1]),(uchar)(d3*tm + mpix[2]) };
							//�ж�ȷ��I J������
							if (t == 0 || t == 1)
							{
								//iΪ�к� jΪ�к�
								if(i>=0&&i<result.rows&&j>=0&&j<result.cols)
								ritr(i, j) += spix*w;

							}
							else
							{
								//jΪ�к� iΪ�к�
								if (j >= 0 && j < result.rows&&i >= 0 && i < result.cols)
								ritr(j, i) += spix*w;
							}
						}
					}
				}

			}
		}
	}
}
