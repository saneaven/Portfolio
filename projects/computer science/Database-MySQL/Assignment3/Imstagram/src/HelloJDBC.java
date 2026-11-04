import java.sql.*;

public class HelloJDBC {
    static final String DB_URL = "jdbc:mysql://localhost:3306/rarebooks";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";
    static final String QUERY = "SELECT author_last_first FROM author";

    public static void main(String[] args) {
        // Open a connection
        try(Connection conn = DriverManager.getConnection(DB_URL, USER, PASS);
            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery(QUERY);) {
            // Extract data from result set
            while (rs.next()) {
                // Retrieve by column name
                System.out.println("first_last: " + rs.getString("author_last_first"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
