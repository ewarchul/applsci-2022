library(tidyverse)
library(here)
library(scales)

get_ew_theme <- function() {
  theme <- ggplot2::theme_bw() +
    ggplot2::theme(
      axis.title = ggplot2::element_text(size = 25, face = "bold"),
      axis.text = ggplot2::element_text(size = 25, face = "bold"),
      legend.text = ggplot2::element_text(size = 25, face = "bold"),
      legend.title = element_blank(),
      legend.position = c(0.90, 0.85),
      legend.background = element_rect(fill = "white", color = "black")
    )
}

plot_line <- function(df, trans = identity) {
  df |>
    ggplot2::ggplot(aes(x = Generation, y = trans(Cost), col = Model)) +
    #    ggplot2::geom_point(aes(shape = Model), alpha = 0.7) +
    ggplot2::geom_path(linetype = "solid", size = 1.6) +
    ggplot2::scale_x_log10(
      breaks = trans_breaks("log10", function(x) 10^x),
      labels = trans_format("log10", math_format(10^.x)),
      limits = c(1, 10000)
    ) +
    get_ew_theme() +
    ylab("Objective function") +
    xlab("Number of generation") +
    ggplot2::scale_colour_brewer(palette = "Dark2")
}


mk_plot <- function(dfr, dim, model, MIP) {
  dfr |>
  dplyr::filter(NetworkSize == dim, Model %in% c(stringr::str_glue("ABC-{model}"), stringr::str_glue("EA-{model}"))) |>
  plot_line() +
    ggplot2::geom_segment(
      aes(x = 1, xend = 10000, y = MIP, yend = MIP, col = stringr::str_glue("MIP-{model}")),
      linetype = "dashed"
    ) +
  ggplot2::theme(legend.position = c(0.9, 0.4))
}

mk_elp_plots <- function() {
  mk_plot(h_dfr, 25, "BLP", 6280)
}

set_feval = function(dfr) {
}

mk_convergence_plots <- function(dfr) {
  blp_dfr = dplyr::filter(dfr, Model %in% c("ABC-BLP", "EA-BLP"))
  elp_dfr = dplyr::filter(dfr, Model %in% c("ABC-ELP", "EA-ELP"))
}

rename_models <- function(dfx) {
  dfx |> 
  dplyr::mutate(
    Model = dplyr::if_else(
      Model == "BA-fm",
      "ABC",
      if_else(Model == "EA-fm", "EA", if_else(
        Model == "EA-sm",
        "EA",
        if_else(
          Model == "BA-sm",
          "ABC",
          if_else(Model %in% c("MIP-fm", "MIP-sm"), "MIP", "ERROR")
        )
      ))
    )
  )
}

