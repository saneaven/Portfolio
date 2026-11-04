import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class ImstagramMainPage extends HttpServlet {

    public void init() throws ServletException {
        // Do required initialization
    }

    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        response.setContentType("text/html");

        RequestDispatcher view = request.getRequestDispatcher("main.html");
        view.forward(request, response);
    }

    public void destroy() {
        // do nothing.
    }
}