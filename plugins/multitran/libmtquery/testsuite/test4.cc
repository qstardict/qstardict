/* test4.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */
#include <iostream>
#include <fstream>

//

#include "btree.hh"
#include "singleton.hh"
#include "path.hh"
#include "dict_path.hh"
//

#include "translation.hh"


int compare_articles(const mt::article& a1,const mt::article& a2)
{
    if (a1.lgk() != a2.lgk())
	return a2.lgk() > a1.lgk();
    else
	return a2.subject() > a1.subject();
}

struct show
{
	show(std::ostream& os): os_(os) {}
	void operator()(const mt::article_set& as)
	{
		if (as.articles_.empty())
		{
		    os_<<as.untranslated_<<" ( langin="<<as.langin_<<" ) - words not found"<<std::endl;
		}
		else
		{
			mt::article_list ar = as.articles_;//NOTE: Windows can made sort only on copy
			std::sort(ar.begin(),ar.end(),compare_articles);
			int prev_lgk = -1;
			std::string prev_subj="x";
			for(size_t i=0;i<ar.size();++i)
			{
				const mt::article& a = ar[i];
				std::string subj = a.subject().empty()?"0":a.subject();

				if (prev_lgk != a.lgk())
				{
					os_<<std::endl<<a.orig()<<",lgk="<<a.lgk()<<std::endl;
					prev_lgk = a.lgk();
					prev_subj = "x";
				}
				if (prev_subj != subj)
				{
					os_<<std::endl<<"\t"<<"subject="<<subj<<" "<<a.translated();
					prev_subj = subj;
				}
				else
					os_<<", "<<a.translated();
			}
			os_<<std::endl;
		}
	}
	std::ostream &os_;
};

int main()
{
	mt::phrase ph;

	ph.push_back("personal");
	ph.push_back("computer");
	ph.push_back("size");
	ph.push_back("frbrpr");
	ph.push_back("stack");
	ph.push_back("overflow");
	ph.push_back("was");
	ph.push_back("tested");
    
/*
	ph.push_back("состоять");
    ph.push_back("из");
    ph.push_back("приготовления");
    ph.push_back("состава");

    ph.push_back("официально");  
    ph.push_back("подтверждать");
*/

	mt::translation tr(ph,mt::english,mt::russian);
	std::cerr<<"translated"<<std::endl;

	std::ofstream os("test4-out.txt");
	std::for_each(tr.asets().begin(),tr.asets().end(),show(os));
	std::cerr<<"output is ready"<<std::endl;

	std::string line;
	std::getline(std::cin,line);
}
