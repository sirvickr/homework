package com.gradle.task;

import java.util.Arrays;

public class Program {
    public static void main(String[] args) {
        Notebook notebook = new Notebook();
        notebook.Load();

        ///args = new String[] { "-add", "3",  "Третья заметка" };
        ///args = new String[] { "-add", "1",  "Первая заметка" };
        ///args = new String[] { "-add", "2",  "Вторая заметка" };
        ///args = new String[] { "-add", "4",  "Четвёртая заметка" };

        // фильтрует сообщения между двумя датами
        ///args = new String[] { "-show", "2020-12-31 15:00:00", "2020-12-31 23:00:00" };
        // фильтрует сообщения между двумя датами, с конкретными ключами
        ///args = new String[] { "-show", "2020-12-31 15:00:00", "2020-12-31 23:00:00", "1",  "3" };

        String cmd = (args.length > 0) ? args[0] : "-show";
        if(cmd.equals("-add")) {
            if(args.length == 3) {
                notebook.Add(args[1], args[2]);
            }
        } else if(cmd.equals("-rm")) {
            if(args.length == 2) {
                notebook.Remove(args[1]);
            }
        } else if(cmd.equals("-show")) {
            if(args.length > 1) {
                notebook.Print(Arrays.copyOfRange(args, 1, args.length));
            } else {
                notebook.Print(new String[0]);
            }
        } else {
            System.out.println("Неизвестная команда");
        }

        notebook.Save();
    }
}
