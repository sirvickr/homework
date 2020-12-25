package com.gradle.task;

public class Program {
    public static void main(String[] args) {
        Notebook notebook = new Notebook();
        notebook.Load();
        ///args = new String[] { "-add", "1",  "FIRST_NOTE" };
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
            notebook.Print();
        } else {
            System.out.println("Неизвестная команда");
        }

        notebook.Save();
    }
}
