/* lgk.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef MT_LGK_HH__
#define MT_LGK_HH__

namespace mt
{
    namespace sparts
    {
	/**
	 * all possible speech parts
	 */
	enum spart_type
	{
	    /* without options */
	    special_part = -1,
	    abbreviation = 1, /**< ���������� */
	    adverb, /**< ������� */
	    interjection, /**< ���������� */
	    conjunction, /**< ���� */
	    article, /**< ������� */
	    numeral, /**< �������������� ������������ */
	    ord_numeral, /**< ���������� ������������ */
	    preposition, /**< ������� */
	    form, /**< ������������ */
	    particle, /**< ������� */
	    predicative, /**< ���������� */
	    /* with options */
	    pronoun = 1<<4, /**< ������ ����������� */
	    adjective = 1<<5, /**< �������������� */
	    noun = 1<<6, /**< ��������������� */
	    verb = 1<<7, /**< ������ */
	    /**< extra options in high bit*/
	    euphemism = 1<<10 /**< �������� */
	};
    }

    namespace adj_bits
    {
	enum options
	{ /**< ��������� ��������������� */
	    sya = 1, /**< ������������� � '��' */
    	    pronoun = 1<<1, /**< ������ͣ���� �������������� */
	    /* bit not used */
	    comparative = 1<<3, /**< ������� ������������� ����� */
	    has_short = 1<<4 /**< ������� �������� ����� */
	};
    }
    
    namespace noun_bits
    {
	enum options
	{ /**< ��������� ����������������*/
	    singular = 1, /**< ������ ������������ ����� */
	    plural = 1<<1, /**< ������ ������������� ����� */
	    normal = singular|plural, /**< ������� (� ����������� � ������������� �����) */
	    animated = 1<<2, /**< ������̣���� */
	    masculine = 1<<3, /**< ������� ���*/
	    feminine = 1<<4, /**< ������� ��� */
	    old_neuter = feminine|masculine, /**< ������� ��� */
	    unchangeable = 1<<5, /**< ������������ */
	    /* ������� ���� */
	    pronoun = 1<<8, /**<  �ݣ ��������� �������� ��� ��������� */
	    neuter = 1<<9 /**< �ݣ ���� ��������� ��� ��� �������� ���� */
	};
    }
    
    namespace verb_bits
    {
	enum options
	{ /**< ��������� ������� */
	    sya = 1, /**< ������ � '��' */
	    participle3 = 1<<1, /**< ��������� 3, '��������' */	    
	    participle4 = 1<<2, /**< ��������� 4, '��������' */
	    perfect = 1<<3, /**< ����������� */
	    imperfect = 1<<4, /**< ������������� */
	    transitive = 1<<5, /**< ���������� */
	    intransitive = 1<<6, /**< ������������ */
	    /* ������� ���� */
	    geborren = 1<<8, /**< ��������� ���������� � �������� ����� */
	    prich = 1<<9 /**< ��������� (����� �������, �������� ��� ��������������) */ //P_PRICH
	};
    }

    int speech_part(int); /**< @return only speech part bit from code */
    
    inline bool is_noun(int lgk) { return (speech_part(lgk)&sparts::noun) == sparts::noun; }
    inline bool is_verb(int lgk) { return (speech_part(lgk)&sparts::verb) == sparts::verb; }
    inline bool is_adjective(int lgk){ return (speech_part(lgk)&sparts::adjective) == sparts::adjective; }
    inline bool is_prich(int lgk)
    {
	    return (!is_noun(lgk)) && 
		   ((lgk&verb_bits::prich) == verb_bits::prich);
    }

    bool is_perfect(int); /**< @check are lgk perfect */
}


#endif
