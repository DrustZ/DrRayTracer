//
//  DrPnt.h
//  DrRayTracer
//
//  Created by 明瑞 on 15/4/22.
//  Copyright (c) 2015年 明瑞. All rights reserved.
//

#ifndef __DrRayTracer__DrPnt__
#define __DrRayTracer__DrPnt__
/**
 *  一个智能计数指针
 */
#define NULL 0
#include <atomic>

template<typename T>
class DrPnt{
    struct DrHolder{
        T* ptn;
        std::atomic_int count;
        
        DrHolder(): ptn(NULL), count(0){}
        DrHolder(T* p): ptn(p), count(1){}
        
        ~DrHolder(){
            if (ptn) delete ptn;
            ptn = NULL;
            count = 0;
        }
    };
    
    DrHolder *m_holder;
    
    void increase_ref(){
        m_holder->count++;
    }
    
    void decrease_ref(){
        if (m_holder && !(--m_holder->count)){
            delete m_holder;
            m_holder = NULL;
        }
    }
    
public:
    DrPnt(T* p): m_holder(new DrHolder(p)){}
    DrPnt(const DrPnt &orig):
    m_holder(orig.m_holder){ increase_ref(); }
    DrPnt& operator=(const DrPnt& orig)
    {
        if (m_holder != orig.m_holder){
            decrease_ref();
            m_holder = orig.m_holder;
            increase_ref();
        }
        return *this;
    }
    T* getPtr() const { return m_holder->ptn; }
    T* operator -> () const { return m_holder->ptn; }
    T& operator * () const { return *m_holder->ptn; }
    
    bool operator == (const DrPnt<T> &p) const
    { return m_holder->ptn == p.m_holder->ptn; }
    
    bool operator == (const T* p) const
    { return m_holder->ptn == p; }
    
    operator bool() const
    { return m_holder->ptn != NULL; }
};
#endif /* defined(__DrRayTracer__DrPnt__) */
