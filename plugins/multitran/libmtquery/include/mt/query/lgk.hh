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
	    abbreviation = 1, /**< сокращение */
	    adverb, /**< наречие */
	    interjection, /**< междометие */
	    conjunction, /**< союз */
	    article, /**< артикль */
	    numeral, /**< количественное числительное */
	    ord_numeral, /**< порядковое числительное */
	    preposition, /**< предлог */
	    form, /**< неизменяемое */
	    particle, /**< частица */
	    predicative, /**< предикатив */
	    /* with options */
	    pronoun = 1<<4, /**< личное местоимение */
	    adjective = 1<<5, /**< прилагательное */
	    noun = 1<<6, /**< существительное */
	    verb = 1<<7, /**< глагол */
	    /**< extra options in high bit*/
	    euphemism = 1<<10 /**< эвфемизм */
	};
    }

    namespace adj_bits
    {
	enum options
	{ /**< параметры прилагательного */
	    sya = 1, /**< употребляется с 'ся' */
    	    pronoun = 1<<1, /**< местоимённое прилагательное */
	    /* bit not used */
	    comparative = 1<<3, /**< наличие сравнительной формы */
	    has_short = 1<<4 /**< наличие короткой формы */
	};
    }
    
    namespace noun_bits
    {
	enum options
	{ /**< параметры существительного*/
	    singular = 1, /**< только единственное число */
	    plural = 1<<1, /**< только множественное число */
	    normal = singular|plural, /**< обычное (и едиственное и множественное число) */
	    animated = 1<<2, /**< одушевлённое */
	    masculine = 1<<3, /**< мужской род*/
	    feminine = 1<<4, /**< женский род */
	    old_neuter = feminine|masculine, /**< средний род */
	    unchangeable = 1<<5, /**< неизменяемое */
	    /* верхние биты */
	    pronoun = 1<<8, /**<  ещё отдельное свойство под причастие */
	    neuter = 1<<9 /**< ещё один отдельный бит для среднего рода */
	};
    }
    
    namespace verb_bits
    {
	enum options
	{ /**< параметры глагола */
	    sya = 1, /**< глагол с 'ся' */
	    participle3 = 1<<1, /**< причастие 3, 'делаемый' */	    
	    participle4 = 1<<2, /**< причастие 4, 'деланный' */
	    perfect = 1<<3, /**< совершенный */
	    imperfect = 1<<4, /**< несовершенный */
	    transitive = 1<<5, /**< переходный */
	    intransitive = 1<<6, /**< непереходный */
	    /* верхние биты */
	    geborren = 1<<8, /**< некоторое исключение в немецком языке */
	    prich = 1<<9 /**< причастие (форма глагола, меняется как прилагательное) */ //P_PRICH
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
