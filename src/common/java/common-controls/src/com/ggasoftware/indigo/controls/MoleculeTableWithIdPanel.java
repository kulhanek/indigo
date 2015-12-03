package com.ggasoftware.indigo.controls;

import java.awt.Color;
import java.awt.Component;
import java.util.Collection;
import javax.swing.JPanel;
import javax.swing.table.DefaultTableColumnModel;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;

public class MoleculeTableWithIdPanel extends JPanel {

    private int _id_column_count = 2;
    private TableCellMouseHandler molecule_cell_handler;

    public MoleculeTableWithIdPanel() {
        initComponents();
        molecule_cell_handler = new TableCellMouseHandler(2);
        table.addMouseListener(molecule_cell_handler);
        table.setShowGrid(true);
        if (table.getGridColor() == table.getBackground()) {
            table.setGridColor(Color.gray);
        }
    }

    public void setIdColumnCount(int count) {
        if (count != 1 && count != 2) {
            throw new RuntimeException("count != 1 && count != 2");
        }

        if (count == 1) {
            final TableColumn column = table.getColumnModel().getColumn(1);
            table.removeColumn(column);
            final TableColumn column0 = table.getColumnModel().getColumn(0);
            column0.setHeaderValue("Id");
            _id_column_count = 1;
            molecule_cell_handler.setTargetColumn(1);
        }
        if (_id_column_count != 1) {
            throw new RuntimeException("Number of ID columns can be specified only once");
        }
    }

    public int getIdColumnCount() {
        return _id_column_count;
    }

    public void setEntityColumnLabel(String label) {
        final TableColumn column = table.getColumnModel().getColumn(_id_column_count);
        column.setHeaderValue(label);
    }

    public String getEntityColumnLabel() {
        final TableColumn column = table.getColumnModel().getColumn(_id_column_count);
        return column.getHeaderValue().toString();
    }

    public void clear() {
        final DefaultTableModel model = (DefaultTableModel) table.getModel();
        while (model.getRowCount() != 0) {
            model.removeRow(model.getRowCount() - 1);
        }
    }

    public void setRowHeight(int height) {
        table.setRowHeight(height);
    }

    public int getRowHeight() {
        return table.getRowHeight();
    }

    public void setObjects(Collection<? extends RenderableObjectWithId> data) {
        clear();

        final DefaultTableModel model = (DefaultTableModel) table.getModel();
        for (RenderableObjectWithId item : data) {
            Object row[] = new Object[3];
            for (int i = 0; i < _id_column_count; i++) {
                row[i] = item.getId(i);
            }
            row[2] = item;
            model.addRow(row);
        }

        // Precalculate id columns widths
        System.out.println("Precalculate id columns widths");
        int widths[] = new int[_id_column_count];
        for (int i = 0; i < _id_column_count; i++) {
            widths[i] = getColumnWidth(i, 2);
        }

        DefaultTableColumnModel colModel = (DefaultTableColumnModel) table.getColumnModel();
        // Lock row width
        for (int i = 0; i < _id_column_count; i++) {
            TableColumn col = colModel.getColumn(i);
            int width = widths[i];
            // Truncate to avoid very long columns
            width = Math.min(150, width);
            col.setMinWidth(width);
            col.setMaxWidth(width);
            col.setPreferredWidth(width);
            col.setWidth(width);
        }
        table.doLayout();
        // Unlock after layout
        for (int i = 0; i < _id_column_count; i++) {
            TableColumn col = colModel.getColumn(i);
            int width = widths[i];
            col.setMinWidth(10);
            col.setMaxWidth(10 * width);
        }
    }

    public int getColumnWidth(int col_index, int margin) {
        DefaultTableColumnModel col_model = (DefaultTableColumnModel) table.getColumnModel();
        TableColumn col = col_model.getColumn(col_index);
        int width = 0;

        // Get width of column header
        TableCellRenderer renderer = col.getHeaderRenderer();
        if (renderer == null) {
            renderer = table.getTableHeader().getDefaultRenderer();
        }

        Component comp = renderer.getTableCellRendererComponent(
                table, col.getHeaderValue(), false, false, 0, 0);
        width = comp.getPreferredSize().width;

        // Get maximum width of column data
        int row_count = table.getRowCount();
        for (int r = 0; r < row_count; r++) {
            // Calculate width for the first 100th, last 100th and some other molecules
            if (r > 100 && r < row_count - 100 && (r % 100) != 0) {
                continue;
            }
            if ((r % 10000) == 0) {
                System.out.println(String.format("%d done", r));
            }

            renderer = table.getCellRenderer(r, col_index);
            comp = renderer.getTableCellRendererComponent(
                    table, table.getValueAt(r, col_index), false, false, r, col_index);
            width = Math.max(width, comp.getPreferredSize().width);
        }

        // Add small margin
        return width + 2 * margin;
    }

    public void addTableCellMouseListener(TableCellMouseListener listener) {
        molecule_cell_handler.addTableCellMouseListener(listener);
    }

    public void removeTableCellMouseListener(TableCellMouseListener listener) {
        molecule_cell_handler.removeTableCellMouseListener(listener);
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jPanel1 = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        table = new javax.swing.JTable();

        table.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Id 1", "Id 2", "Molecule"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.String.class, java.lang.Object.class
            };
            boolean[] canEdit = new boolean [] {
                true, true, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        table.setAutoResizeMode(javax.swing.JTable.AUTO_RESIZE_LAST_COLUMN);
        table.setRowSelectionAllowed(false);
        table.getTableHeader().setReorderingAllowed(false);
        jScrollPane2.setViewportView(table);
        table.getColumnModel().getColumn(0).setCellEditor(new com.ggasoftware.indigo.controls.MultilineCenteredTableCellEditor(false));
        table.getColumnModel().getColumn(0).setCellRenderer(new com.ggasoftware.indigo.controls.MultilineCenteredTableCellRenderer());
        table.getColumnModel().getColumn(1).setCellEditor(new com.ggasoftware.indigo.controls.MultilineCenteredTableCellEditor(false));
        table.getColumnModel().getColumn(1).setCellRenderer(new com.ggasoftware.indigo.controls.MultilineCenteredTableCellRenderer());
        table.getColumnModel().getColumn(2).setCellRenderer(new com.ggasoftware.indigo.controls.MolRenderer());

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 281, Short.MAX_VALUE)
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 292, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 281, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 292, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTable table;
    // End of variables declaration//GEN-END:variables
}
