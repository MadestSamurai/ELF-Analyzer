package elf.reader;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class FileChooser extends JFrame implements ActionListener {
    JButton open = null;

    public static void main(String[] args) {
        new FileChooser();
    }

    public FileChooser() {
        super("ELF Choose");
        open = new JButton("open ELF File");
        open.setSize(100,30);
        this.add(open);
        this.setBounds(500, 500, 300, 140);
        this.setVisible(true);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        open.addActionListener(this);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        JFileChooser jfc = new JFileChooser();
        jfc.setFileSelectionMode(JFileChooser.FILES_ONLY);
        jfc.showDialog(new JLabel(), "选择");
        File file = jfc.getSelectedFile();
        new GUI(file.getAbsolutePath());
    }
}