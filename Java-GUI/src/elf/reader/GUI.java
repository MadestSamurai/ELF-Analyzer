package elf.reader;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class GUI extends JFrame {
    private JFrame frame = new JFrame();

    public GUI(String filename) {
        init(filename);
        frame.setTitle("ELF Reader");
        frame.setBounds(100, 100, 1500, 900);
        frame.setResizable(true);
        frame.setVisible(true);
        frame.setLayout(new FlowLayout());
    }

    private void init(String filename) {
        try {
            Runtime.getRuntime().exec("untitled.exe "+filename);

            FileInputStream inputStream = new FileInputStream("result.txt");
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));

            String str;
            int area = 0;

            JLabel label[] = new JLabel[21];
            JTextField field[] = new JTextField[21];
            int elfCnt = 0;

            JPanel textPanel = new JPanel();
            Border titleBorder1 = BorderFactory.createTitledBorder("ELF Header");
            textPanel.setBorder(titleBorder1);
            textPanel.setLayout(new GridLayout(19,2));
            textPanel.setSize(600,400);


            String[] name = {"[Nr]","Name","Type","Address","Off","Size","ES","Flg","Lk","Inf","Al"};
            String[][] data = {{"[ 0]"," ","NULL","0000000000000000","000000","000000","00","000","00","000","00"}};
            DefaultTableModel tableM1 = new DefaultTableModel(data, name);
            JTable table1 = new JTable(tableM1);
            JScrollPane jsp = new JScrollPane(table1);
            JPanel sectionPanel = new JPanel();
            sectionPanel.add(jsp);
            jsp.setSize(800, 500);
            sectionPanel.setSize(800,500);
            Border titleBorder2 = BorderFactory.createTitledBorder("Section Headers");
            sectionPanel.setBorder(titleBorder2);

            String[] name2 = {"Num","Value","Size","Type","Bind","Vis","Ndx","Name"};
            String[][] data2 = null;
            DefaultTableModel tableM2 = new DefaultTableModel(data2, name2);
            JTable table2 = new JTable(tableM2);
            JScrollPane jsp2 = new JScrollPane(table2);
            JPanel symtabPanel = new JPanel();
            symtabPanel.add(jsp2);
            jsp.setSize(800, 500);
            symtabPanel.setSize(800,500);
            Border titleBorder3 = BorderFactory.createTitledBorder(".symtab");
            symtabPanel.setBorder(titleBorder3);

            DefaultTableModel tableM3 = new DefaultTableModel(data2, name2);
            JTable table3 = new JTable(tableM3);
            JScrollPane jsp3 = new JScrollPane(table3);
            JPanel dynsymPanel = new JPanel();
            dynsymPanel.add(jsp3);
            jsp.setSize(800, 300);
            dynsymPanel.setSize(800,300);
            Border titleBorder4 = BorderFactory.createTitledBorder(".dynsym");
            dynsymPanel.setBorder(titleBorder4);

            String[] name3 = {"Type","Offset","VirtAddress","PhysicAddress","FileSize","MemorySize","Flg","Align"};
            String[][] data3 = null;
            DefaultTableModel tableM4 = new DefaultTableModel(data3, name3);
            JTable table4 = new JTable(tableM4);
            JScrollPane jsp4 = new JScrollPane(table4);
            JPanel programPanel = new JPanel();
            programPanel.add(jsp4);
            jsp.setSize(800, 500);
            programPanel.setSize(800,500);
            Border titleBorder5 = BorderFactory.createTitledBorder("Program Headers");
            programPanel.setBorder(titleBorder5);

            String[] name4 = {"Segment","Sections"};
            DefaultTableModel tableM5 = new DefaultTableModel(data3, name4);
            JTable table5 = new JTable(tableM5);
            JScrollPane jsp5 = new JScrollPane(table5);
            JPanel StoSPanel = new JPanel();
            StoSPanel.add(jsp5);
            jsp.setSize(800, 500);
            StoSPanel.setSize(800,500);
            Border titleBorder6 = BorderFactory.createTitledBorder("Section to Segment mapping");
            StoSPanel.setBorder(titleBorder6);

            while ((str = bufferedReader.readLine()) != null) {
                System.out.println(str);
                if (str.contains("ELF Header:")) {
                    area = 1;
                    continue;
                }
                if (str.contains("Section Headers:")) {
                    area = 2;
                    continue;
                }
                if (str.contains("Symbol table '.symtab'")) {
                    area = 3;
                    continue;
                }
                if (str.contains("Symbol table '.dynsym'")) {
                    area = 4;
                    continue;
                }
                if (str.contains("Program Headers:")) {
                    area = 5;
                    continue;
                }
                if (str.contains("Section to Segment mapping:")) {
                    area = 6;
                    continue;
                }

                if (str.contains("There are") || str.isEmpty()) continue;

                if (area == 1) {
                    label[elfCnt] = new JLabel(str.split(":")[0]);
                    field[elfCnt] = new JTextField(str.split(":")[1].trim());
                    field[elfCnt].setColumns(5);
                    field[elfCnt].setEditable(false);
                    textPanel.add(label[elfCnt]);
                    textPanel.add(field[elfCnt]);
                    elfCnt++;
                }

                if(area == 2) {
                    if(str.contains("[Nr]") || str.contains("[ 0]")) {
                        continue;
                    }
                    String[] row = str.trim().replaceAll("\\[ ","\\[").split("\\s+");
                    tableM1.addRow(row);
                }

                if(area == 3) {
                    if(str.contains("Num:")) {
                        continue;
                    }
                    String[] row = str.trim().replaceAll(": "," ").split("\\s+");
                    tableM2.addRow(row);
                }

                if(area == 4) {
                    if(str.contains("Num:")) {
                        continue;
                    }
                    String[] row = str.trim().replaceAll(": "," ").split("\\s+");
                    tableM3.addRow(row);
                }

                if(area == 5) {
                    if(str.contains("Type")) {
                        continue;
                    }
                    String[] row = str.trim().split("\\s+");
                    tableM4.addRow(row);
                }

                if(area == 6) {
                    if(str.contains("Segment")) {
                        continue;
                    }
                    String[] row = str.trim().split("\\s+",2);
                    tableM5.addRow(row);
                }
            }
            frame.getContentPane().add(textPanel);
            frame.getContentPane().add(sectionPanel);
            frame.getContentPane().add(symtabPanel);
            frame.getContentPane().add(dynsymPanel);
            frame.getContentPane().add(programPanel);
            frame.getContentPane().add(StoSPanel);
            inputStream.close();
            bufferedReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
