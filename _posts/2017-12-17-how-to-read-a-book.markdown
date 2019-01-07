---
layout: post
title:  "How to Read a book"
subtitle: "如何阅读一本书"
date:   2017-12-17 15:38:59 +0800
categories: [read]
---

《如何阅读一本书》是由美国哲学家莫蒂默·杰尔姆·阿德勒于1940年所著，并于1972年和查尔斯·范多伦重新修订。它将阅读分做四个层次基础阅读、检视阅读、分析阅读、主题阅读。我试着用代码来解释如何阅读一本书。


```c
/* 基础阅读: 理解这个句子在说什么? */ 
func basic_read(sentence) {
    read_and_understand(sentence);
}

/* 检视阅读: 这本书是否值得仔细阅读? */
func inspectional_read() {
    int threshold;
    int is_valued;
    
    chosen_chapters = read_content();
    for chapter in chosen_chapters:
        chosen_sentences = read_chapter();
        for sentence in chosen_sentences:
            basic_read(sentece);
            
    if (is_valued > threshold)
        return 1;
    else
        return 0;
}

/* 分析阅读 */
func analytical_read() {
    /* 架构性: 找出一本书在谈写什么 */
    keywords = tags = classify_topic();
    summary = explain_whole_book_using_simplest_abstract();
    association = find_correlation_between_primary_parts();
    theme_questions = determine_what_question_author_want_solve_();
    
    /* 诠释性: 这本书的详细内容是什么? 作者是如何写出来的? */
    explain_keywords(keywords);
    gist = catch_gist_from_summay(summary);
    find_supports_to_enrich_association_diagram(association);
    determine_questiones_solve_or_not(theme_questions);
    
    /* 评论性: 这是真实的、有道理的吗? 这本书跟你有什么关系 */
    set(comment, UN_COMPETITIVE)        // 不要争强好胜
    comment = [opinion for opinion in views ...
    ... if (opinion in real_knowledge)] // 区分真正的知识和个人观点
    // 批评的标准: 证明作者知识不足或错误，或不合逻辑，或分析与理由不充分 */
}

/* 主题阅读: 讨论某个特定的主题 */
func topic_read() {
    /* init phase */
    book_list = chose_by_book_title();
    book_list = inspectional_read(book_list);
    
    /* run phase */
    topic_chapters = analytical_read(book_list);
    define_keywords(keywords);      // 与作者对概念定义达成一致
    new_gist = determine_nonaligned_gist(gist); // 主旨
    questions = list_questions_in_gist(gist);   // 核心问题
    important_questions = determine_important(questions);
    comment = analyse(important_questions);     // 讨论
    conclusion = analyse(comment);              // 结论
}
```