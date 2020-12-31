package com.gradle.task;

import static org.junit.jupiter.api.Assertions.*;

class NotebookTest {

    @org.junit.jupiter.api.Test
    void add() {
        String[] notes = {
                "First note",
                "Second note",
                "Third note"
        };
        Notebook notebook = new Notebook();
        for(int i = 0; i < notes.length; i++) {
            String key = Integer.toString(i);
            String note = notes[i];
            notebook.Add(key, note);
            assertEquals(i + 1, notebook.Content().size());
            assertTrue(notebook.Content().containsKey(key));
            assertEquals(note, notebook.Content().get(key).note);
        }
    }
}