#include <iostream>

#include "scorer.h"
#include "lm/model.hh"
#include "util/tokenize_piece.hh"
#include "util/string_piece.hh"

using namespace lm::ngram;

Scorer::Scorer(float alpha, float beta, std::string lm_model_path) {
    this->_alpha = alpha;
    this->_beta = beta;
    this->_language_model = new Model(lm_model_path.c_str());
}

Scorer::~Scorer(){
   delete (Model *)this->_language_model;
}

inline void strip(std::string str, char ch=' ') {
    if (str.size() == 0) return;
    int start  = 0;
    int end = str.size()-1;
    for (int i=0; i<str.size(); i++){
        if (str[i] == ch) {
            start ++;
        } else {
            break;
        }
    }
    for (int i=str.size()-1; i>=0; i--) {
        if  (str[i] == ch) {
            end --;
        } else {
            break;
        }
    }
    
    if (start == 0 && end == str.size()-1) return;
    if (start > end) {
        std::string emp_str;
        str = emp_str;
    } else {
        str = str.substr(start, end-start+1);
    }
}

int Scorer::word_count(std::string sentence) {
    strip(sentence);
    int cnt = 0;
    for (int i=0; i<sentence.size(); i++) {
        if (sentence[i] == ' ') {
            cnt ++;
        }
    }
    if (cnt > 0) cnt ++;
    return cnt;
}

float Scorer::language_model_score(std::string sentence) {
    Model *model = (Model *)this->_language_model;
    State state, out_state;
    lm::FullScoreReturn ret;
    float score = 0.0;
    state = model->BeginSentenceState();

    for (util::TokenIter<util::SingleCharacter, true> it(sentence, ' '); it; ++it){
        lm::WordIndex vocab = model->GetVocabulary().Index(*it);
        ret = model->FullScore(state, vocab, out_state);
        score += ret.prob;
        std::cout<<ret.prob<<"\n";
        state = out_state;
        ret = model->FullScore(state, model->GetVocabulary().EndSentence(), out_state);
        score += ret.prob; 
    }
    return score;
}

float Scorer::get_score(std::string sentence) {
    int word_cnt = word_count(sentence);
    float lm_score = language_model_score(sentence);

    float final_score = pow(lm_score, _alpha) * pow(word_cnt, _beta);
    return final_score;
}
/*
int main()
{
    Scorer ext_scorer(1.0, 1.0, "/Users/liuyibing01/Projects/paddle_work/models/deep_speech_2/data/1Billion.klm");
    std::cout<< ext_scorer.word_count("ha ha ha")<<std::endl;
    std::cout<< ext_scorer.language_model_score("I am fine")<<std::endl;
    return 0;
}*/
