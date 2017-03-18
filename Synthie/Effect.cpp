/**
 * \file Effect.cpp
 *
 * \author Xingchen Xiao
 */


#include "stdafx.h"
#include "Effect.h"

#include "Note.h"

CEffect::CEffect()
{
	m_queueL.resize(MAXQUEUESIZE);
	m_queueR.resize(MAXQUEUESIZE);
}


CEffect::~CEffect()
{

}


void CEffect::SetNote(CNote *note)
{
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	for (int i = 0; i < len; i++)
	{
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		CComBSTR name;
		attrib->get_nodeName(&name);

		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "wet")
		{
			value.ChangeType(VT_R8);
			SetWet(value.dblVal);
		}
		else if (name == "dry")
		{
			value.ChangeType(VT_R8);
			SetDry(value.dblVal);
		}
		else if (name == "delay")
		{
			value.ChangeType(VT_R8);
			SetDelay(value.dblVal);
		}
		else if (name == "threshold")
		{
			value.ChangeType(VT_R8);
			SetThreshold(value.dblVal);
		}
	}
}