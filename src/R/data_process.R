library(tidyverse)
library(here)
library(magrittr)

avg_dfr <- function(df1, df2, col = "Cost") {
  avg <- 0.5 * (dplyr::pull(df1, col) + dplyr::pull(df2, col))
  df1[[col]] <- avg
  df1
}

avg_dataset <- function(dataset) {
  dataset |>
    dplyr::group_by(Repetition) |>
    dplyr::group_map(\(df, g) {
      df
    }) |>
    purrr::reduce(avg_dfr)
}

avg_solver_ds <- function(dataset) {
  dataset |>
    dplyr::group_by(Model, NetworkSize) |>
    dplyr::group_modify(\(df, ...) {
      avg_dataset(df)
    }) |>
    dplyr::ungroup()
}
