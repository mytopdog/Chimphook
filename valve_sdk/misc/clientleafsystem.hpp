#include "../interfaces/IClientRenderable.hpp"

struct RenderableInfo_t
{
	IClientRenderable* m_pRenderable;
	int					m_RenderFrame;
	int					m_RenderFrame2;
	int					m_EnumCount;
	int					m_TranslucencyCalculated;
	unsigned int		m_LeafList;
	unsigned int		m_RenderLeaf;
	unsigned char		m_Flags;
	unsigned char		m_Flags2;
	unsigned char		m_RenderGroup;
	unsigned short		m_FirstShadow;
	short m_Area;
	signed char			m_TranslucencyCalculatedView;
};