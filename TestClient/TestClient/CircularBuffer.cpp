#include "stdafx.h"
#include "CircularBuffer.h"


CircularBuffer::CircularBuffer(size_t capacity)
  : mBeginIndex(0), mEndIndex(0), mCurrentSize(0), mCapacity(capacity)
{
	mData = new char[capacity] ;
}

CircularBuffer::~CircularBuffer()
{
	delete [] mData ;
}

bool CircularBuffer::Write(const char* data, size_t bytes)
{
	if (bytes == 0)
		return false ;

	/// �뷮 ����
	if ( bytes > mCapacity - mCurrentSize )
		return false ;
	
	// �ٷ� ���� ������ ���
	//////////////////////////////////////////////////////////////////////////
	// ���κ� = mCapacity
	// ���� �����Ͱ� �� �ִ� �� �κ� = mEndIndex
	//
	// ���� ������ �� �� �ִ� ���κк��� ���� ������ ���� ���� ���� - 1
	// = mCapacity - mEndIndex
	//////////////////////////////////////////////////////////////////////////
	if ( bytes <= mCapacity - mEndIndex )
	{
		memcpy(mData + mEndIndex, data, bytes) ;
		mEndIndex += bytes ;

		if ( mEndIndex == mCapacity )
			mEndIndex = 0 ;
		// �����Ͱ� �� ���� mEndIndex�� 0���� ��������
		// ��ŧ�� ť ����
	}
	// �ɰ��� ��� �� ���
	else
	{
		// �޺κ� ���� ���� - ���� �ּ� - 1 ����
		size_t size1 = mCapacity - mEndIndex ;
		memcpy(mData + mEndIndex, data, size1) ;
		
		// �޺κ� �� ä������ ������ �����ʹ� ���� �κк��� �պ��� ä������
		size_t size2 = bytes - size1 ;
		memcpy(mData, data + size1, size2) ;
		mEndIndex = size2 ;
	}

	mCurrentSize += bytes ;

	return true ;
}

bool CircularBuffer::Read(char* data, size_t bytes)
{
	if (bytes == 0)
		return false ;

	if ( mCurrentSize < bytes )
		return false ;

	/// �ٷ� �ѹ��� �о� �� �� �ִ� ���
	//////////////////////////////////////////////////////////////////////////
	// ������ ���κ� = mCapacity 
	// ���� �����Ͱ� �� �ִ� ���ۺκ� = mBeginIndex
	//
	// ������ mCurrentSize < bytes �̴ϱ�
	// bytes <= mCapacity - mBeginIndex ��� ������ �� �ִ� ���ۺκк��� ���� ������
	// �� �� �ִ� ������ �ȿ��� �� ���� bytes�� �ܾ� �� �� �ְ���
	//////////////////////////////////////////////////////////////////////////
	if ( bytes <= mCapacity - mBeginIndex )
	{
		memcpy(data, mData + mBeginIndex, bytes) ;
		mBeginIndex += bytes ;

		if ( mBeginIndex == mCapacity )
			mBeginIndex = 0 ;
		// ��ŧ�� ť ����
	}
	/// �о�� �����Ͱ� �ɰ��� �ִ� ���
	else
	{
		// �̰� ���ۺκк��� ���� �޸��� �� ������
		size_t size1 = mCapacity - mBeginIndex ;
		memcpy(data, mData + mBeginIndex, size1) ;

		// �̰� ���� �޸� ������ġ���� ������ ���� ���������
		size_t size2 = bytes - size1 ;
		memcpy(data + size1, mData, size2) ;
		mBeginIndex = size2 ;
	}

	mCurrentSize -= bytes ;

	return true ;
}

//////////////////////////////////////////////////////////////////////////
// �����͸� �б⸸ �ϹǷ� mBeginIndex, mEndIndex, mCurrentSize ��
// ���� ���� ������ ����.
//////////////////////////////////////////////////////////////////////////

// ������ ��� �ִ� ��ŭ ��°�� Peek
void CircularBuffer::Peek(char* data)
{
	/// �ٷ� �ѹ��� �о� �� �� �ִ� ���
	if ( mCurrentSize <= mCapacity - mBeginIndex )
	{
		memcpy(data, mData + mBeginIndex, mCurrentSize) ;
	}
	/// �о�� �����Ͱ� �ɰ��� �ִ� ���
	else
	{
		size_t size1 = mCapacity - mBeginIndex ;
		memcpy(data, mData + mBeginIndex, size1) ;

		size_t size2 = mCurrentSize - size1 ;
		memcpy(data + size1, mData, size2) ;
	}
}

// ������ ũ�� ��ŭ Peek()
bool CircularBuffer::Peek(char* data, size_t bytes)
{
	if (bytes == 0)
		return false ;

	if ( mCurrentSize < bytes )
		return false ;

	/// �ٷ� �ѹ��� �о� �� �� �ִ� ���
	if ( bytes <= mCapacity - mBeginIndex )
	{
		memcpy(data, mData + mBeginIndex, bytes) ;
	}
	/// �о�� �����Ͱ� �ɰ��� �ִ� ���
	else
	{
		size_t size1 = mCapacity - mBeginIndex ;
		memcpy(data, mData + mBeginIndex, size1) ;

		size_t size2 = bytes - size1 ;
		memcpy(data + size1, mData, size2) ;
	}

	return true ;
}


bool CircularBuffer::Consume(size_t bytes)
{
	if (bytes == 0)
		return false ;

	if ( mCurrentSize < bytes )
		return false ;

	/// �ٷ� �ѹ��� ������ �� �ִ� ���
	if ( bytes <= mCapacity - mBeginIndex )
	{
		mBeginIndex += bytes ;

		if ( mBeginIndex == mCapacity )
			mBeginIndex = 0 ;
	}
	/// ������ �����Ͱ� �ɰ��� �ִ� ���
	else
	{
		size_t size2 = bytes + mBeginIndex - mCapacity ;
		//////////////////////////////////////////////////////////////////////////
		// ������ �����Ͱ� �ɰ��� �ִٴ� �̾߱��
		// mBeginIndex + bytes �ϸ� ��ŧ�� ����ó�� �Ѿ�� ���̹Ƿ�
		// ��ⷯ ȿ���� �ֱ� ���� -mCapacity ����� ��
		//////////////////////////////////////////////////////////////////////////
		mBeginIndex = size2 ;
	}

	mCurrentSize -= bytes ;

	return true ;

}