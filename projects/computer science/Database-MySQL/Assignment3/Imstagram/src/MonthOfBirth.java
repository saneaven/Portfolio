import java.io.*;
import java.sql.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class MonthOfBirth extends HttpServlet {
    static final String DB_URL = "jdbc:mysql://localhost:3306/assignment_3";
    static final String USER = "root";
    static final String PASS = "q1w2e3r4!";

    public void init() throws ServletException {
        // Do required initialization
    }

    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        String title = "Database Result";

        String docType =
                "<!doctype html public \"-//w3c//dtd html 4.0 " + "transitional//en\">\n";

        out.println(docType +
                "<html>\n" +
                "<head><title>" + title + "</title></head>\n" +
                "<body bgcolor = \"#f0f0f0\">\n" +
                "<h1 align = \"center\">" + title + "</h1>\n");

        try {
            // Register JDBC driver
            Class.forName("com.mysql.jdbc.Driver");

            Connection conn = DriverManager.getConnection(DB_URL, USER, PASS);
            Statement stmt = conn.createStatement();
            String query = "SELECT COUNT(*), MONTH(birthday) FROM User WHERE MONTH(birthday) is not null group by MONTH(birthday) order by 1 desc";
            ResultSet rs = stmt.executeQuery(query);
            int monthOfMostFriend = 0;
            int monthOfLeastFriend = 0;
            int totalFriendsWithMonthOfBirth = 0;

            // Extract data from result set
            while (rs.next()) {
                // Retrieve by column name
                int count = rs.getInt(1);
                int month = rs.getInt(2);
                if (rs.isFirst())
                    monthOfMostFriend = month;
                if (rs.isLast())
                    monthOfLeastFriend = month;
                totalFriendsWithMonthOfBirth += count;
            }
            out.println("Month of most users:"+monthOfMostFriend+"<br>");
            out.println("Month of least users:"+monthOfLeastFriend+"<br>");


            // Get the names of friends born in the "most" month
            out.println("Here are friends born in most month:");
            rs = stmt.executeQuery("SELECT user_id, first_name, last_name from user where MONTH(birthday)="+monthOfMostFriend);
            while(rs.next()) {
                Long uid = rs.getLong(1);
                String firstName = rs.getString(2);
                String lastName = rs.getString(3);
                out.println(firstName+", "+lastName+", "+uid+"<br>");
            }
            out.println("In total, we have "+totalFriendsWithMonthOfBirth+" friends having their month of birth! <br>");
            out.println("</body></html>");

            rs.close();
            stmt.close();
            conn.close();
        } catch (SQLException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
