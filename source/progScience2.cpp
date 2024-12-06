#include "tests.h" 

int ExampleOfUse() {
    Subject subj;
    Wrapper replace_wrapper(&subj, &Subject::replace, { {"old_word", ""}, {"new_word", ""} });
    Engine engine;
    engine.register_command("replace", replace_wrapper);

    FileWrapper fileWrapper("document.txt");

    engine.execute("replace", { { "old_word", "Hellj" }, { "new_word", "Hello" } }, fileWrapper);
    return 0;
}

int main() {
    test_replace();
    test_find();
    test_capitalize_words();
    test_number_sentences();
    test_capitalize_first_letter_of_sentence();
    return 0;
}
