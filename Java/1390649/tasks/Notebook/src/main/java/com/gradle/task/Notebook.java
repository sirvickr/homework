package com.gradle.task;

import java.io.*;
import java.util.HashMap;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

class Data {
    public HashMap<String, String> notes = new HashMap<>();
}

public class Notebook {
    // конструктор по умолчанию
    public Notebook() {
        builder.setPrettyPrinting();
    }

    // прочитать данные
    public void Load() {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            System.out.println("file " + fileName + " opened");
            String json = "";
            String line;
            while ((line = br.readLine()) != null) {
                json = json + line;
            }
            Gson gson = builder.create();
            data = gson.fromJson(json, Data.class);
            System.out.println("Загружено " + data.notes.size() + " заметок");
        } catch(IOException ex) {
            System.out.println("Файл с данными отсутствует");
        }
    }

    // сохранить данные
    public void Save() {
        Gson gson = builder.create();
        String json = gson.toJson(data);
        try (FileWriter writer = new FileWriter(fileName, false)) {
            writer.write(json);
            writer.append('\n');
        } catch(IOException ex) {
            System.out.println(ex.getMessage());
        }
        System.out.println("Сохранено " + data.notes.size() + " заметок");
    }

    // добавить заметку
    public void Add(String key, String note) {
        data.notes.put(key, note);
        System.out.println("Добавлена заметка " + key + ": " + note);
    }

    // удалить заметку
    public void Remove(String key) {
        if(data.notes.containsKey(key)) {
            data.notes.remove(key);
            System.out.println("Удалена заметка " + key);
        } else {
            System.out.println("Заметки " + key + " не существует");
        }
    }

    // вывести все заметки на экран
    public void Print() {
        System.out.println("Количество заметок: " + data.notes.size());
        for (String key : data.notes.keySet()) {
            System.out.println(key + "\t" + data.notes.get(key));
        }
    }

    // доступ к содержимому
    public HashMap<String, String> Content() {
        return data.notes;
    }

    private String fileName = "notebook.json";
    private Data data = new Data();
    private final GsonBuilder builder = new GsonBuilder();
}
