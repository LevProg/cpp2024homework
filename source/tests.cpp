#pragma once
#include "tests.h"

void test_replace() {
    Subject subj;
    Wrapper::ArgsMap args = { {"old_word", "hello"}, {"new_word", "hi"} };
    Wrapper replaceWrapper(&subj, &Subject::replace, {});
    FileWrapper fileWrapper("document.txt");
    fileWrapper.write("hello world, hello everyone");
    replaceWrapper.execute(args, fileWrapper);
    std::string content = fileWrapper.read();
    assert(content == "hi world, hi everyone" && "Test failed for 'replace' method");
    std::cout << "Test passed for 'replace' method" << std::endl;
}

void test_find() {
    Subject subj;
    Wrapper::ArgsMap args = { {"phrase", "hello"} };
    Wrapper findWrapper(&subj, &Subject::find, {});
    FileWrapper fileWrapper("document.txt");
    fileWrapper.write("hello world");
    findWrapper.execute(args, fileWrapper);
    std::string content = fileWrapper.read();
    assert(content == "Found hello\nhello world" && "Test failed for 'find' method");
    std::cout << "Test passed for 'find' method" << std::endl;
}

void test_capitalize_words() {
    Subject subj;
    Wrapper::ArgsMap args = {};
    Wrapper capitalizeWrapper(&subj, &Subject::capitalize_words, {});
    FileWrapper fileWrapper("document.txt");
    fileWrapper.write("hello world");
    capitalizeWrapper.execute(args, fileWrapper);
    std::string content = fileWrapper.read();
    assert(content == "Hello World" && "Test failed for 'capitalize_words' method");
    std::cout << "Test passed for 'capitalize_words' method" << std::endl;
}

void test_number_sentences() {
    Subject subj;
    Wrapper::ArgsMap args = {};
    Wrapper numberWrapper(&subj, &Subject::number_sentences, {});
    FileWrapper fileWrapper("document.txt");
    fileWrapper.write("This is a sentence. This is another sentence.");
    numberWrapper.execute({ args }, fileWrapper);
    std::string content = fileWrapper.read();
    assert(content == "1. This is a sentence. \n2. This is another sentence."
        && "Test failed for 'number_sentences' method");
    std::cout << "Test passed for 'number_sentences' method" << std::endl;
}

void test_capitalize_first_letter_of_sentence() {
    Subject subj;
    Wrapper::ArgsMap args = {};
    Wrapper capitalizeFirstLetterWrapper(&subj, &Subject::capitalize_first_letter_of_sentence, {});
    FileWrapper fileWrapper("document.txt");
    fileWrapper.write("hello world. how are you? i am fine.");
    capitalizeFirstLetterWrapper.execute(args, fileWrapper);
    std::string content = fileWrapper.read();
    assert(content == "Hello world. How are you? I am fine." && "Test failed for 'capitalize_first_letter_of_sentence' method");
    std::cout << "Test passed for 'capitalize_first_letter_of_sentence' method" << std::endl;
}
