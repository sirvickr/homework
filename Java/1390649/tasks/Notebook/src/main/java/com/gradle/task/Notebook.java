package com.gradle.task;

import java.io.*;
import java.util.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

class Message {
    public Message(String date, String note) {
        this.date = date;
        this.note = note;
    }
    public String date;
    public String note;
}

class Data {
    public HashMap<String, Message> notes = new HashMap<>();
}

public class Notebook {
    // конструктор по умолчанию
    public Notebook() {
        builder.setPrettyPrinting();
    }

    // доступ к содержимому
    public HashMap<String, Message> Content() {
        return data.notes;
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
        data.notes.put(key, new Message(dateFormat.format(new Date()), note));
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

    // вывести отфильтрованные по датам и ключам заметки на экран
    public void Print(String[] filters) {
        TreeMap<Date, String> messages = new TreeMap<>();
        Date since = null;
        Date until = null;
        if(filters.length >= 2) {
            try {
                since = dateFormat.parse(filters[0]);
                until = dateFormat.parse(filters[1]);
                System.out.println("since \"" + dateFormat.format(since) + "\" until \"" + dateFormat.format(until) + "\"");
            } catch (ParseException e) {
                System.out.println("Недопустимый формат даты, должен быть такой: " + datePattern);
            }
        }
        for (String key : data.notes.keySet()) {
            Message item = data.notes.get(key);
            try {
                Date date = dateFormat.parse(item.date);
                // true если либо фильтр не задан, либо дата не попадает в него; иначе false
                boolean matchDate = since == null || until == null ||
                       (date.compareTo(since) >= 0 && date.compareTo(until) <= 0);
                boolean matchKey = filters.length < 3 || Arrays.asList(filters).contains(key);
                if(matchDate && matchKey) {
                    messages.put(date, item.date + "\t[" + key + "]\t" + item.note);
                }
            } catch (ParseException e) {
                System.out.println("Недопустимый формат даты в сообщении (" + item.date + "), должен быть такой: " + datePattern);
            }
        }
        Print(messages);
    }

    // вывести заметки, отсортированные по дате в возрастающем порядке
    private void Print(TreeMap<Date, String> messages) {
        System.out.println("Количество заметок: " + messages.size());
        for (Date date : messages.keySet()) {
            System.out.println(messages.get(date));
        }
    }

    private String fileName = "notebook.json";
    private Data data = new Data();
    private final GsonBuilder builder = new GsonBuilder();
    private String datePattern = "yyyy-MM-dd HH:mm:ss";
    private SimpleDateFormat dateFormat = new SimpleDateFormat(datePattern);
}
