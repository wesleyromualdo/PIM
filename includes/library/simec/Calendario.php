<?php

/**
 * Class de criação de calendario
 *
 * @author  saulocorreia
 *
 * @package Simec\View
 *
 */
class Simec_Calendario
{
    // Configuração Inicial
    private $cell_border;
    private $today;
    private $show_days;
    private $weekstartson = 0;
    private $year;
    private $month;
    private $show_previous;
    private $show_next;
    private $language = "pt";
    private $script;

    private $legend = [];
    private $action;
    private $action_nav;
    private $data = [];

    //------------------------//

    private $grupoCores = [];

    /**
     * Calendario constructor.
     *
     * @param int|null $ano com 4 digitos (YYYY)
     * @param int|null $mes de 1 a 12
     */
    function __construct ($ano = null, $mes = null, $action_nav = 'carregazabutocalendar')
    {
        $this->year = $ano ? $ano : date('Y');
        $this->month = $mes ? $mes : date('m');

        $this->cell_border = (bool) true;
        $this->today = (bool) true;
        $this->show_days = (bool) true;
        $this->action_nav = $action_nav;
        $this->weekstartson = 0;
    }

    private static function filtroArray ($value)
    {
        if (is_null($value) || (is_array($value) && count($value) == 0))
        {
            return false;
        }

        return true;
    }

    /**
     * Marcar o dia de hoje (default => true)
     *
     * @param bool $today
     * @return $this
     */
    public function mostrarToday ($today)
    {
        $this->today = $today;

        return $this;
    }

    /**
     * Mostrar os dias (default => true)
     *
     * @param bool $show_days
     * @return $this
     */
    public function mostrarDias ($show_days)
    {
        $this->show_days = $show_days;

        return $this;
    }

    /**
     * A semana começa em que dia
     * 0 = Domingo (default)
     * 1 = Segunda
     * 2 = Terça
     * 3 = Quarta
     * 4 = Quinta
     * 5 = Sexta
     * 6 = Sabado
     *
     * @param int $weekstartson
     * @return $this
     */
    public function semanaComecaEm ($weekstartson)
    {
        $this->weekstartson = $weekstartson;

        return $this;
    }

    /**
     *
     * @param int $show_previous
     * @return $this
     */
    public function mostrarQuantosMesesAntes ($show_previous)
    {
        $this->show_previous = $show_previous;

        return $this;
    }

    /**
     * @param int $show_next
     * @return $this
     */
    public function mostrarQuantosMesesDepois ($show_next)
    {
        $this->show_next = $show_next;

        return $this;
    }

    /**
     * @param bool $cell_border
     * @return $this
     */
    public function mostrarBordasVerticais ($cell_border)
    {
        $this->cell_border = $cell_border;

        return $this;
    }

    /**
     * Adiciona um objeto de legenda. é possivel inserir mais de um.
     * Exemplo:
     * [
     * {type: "text", label: "Special event", badge: "00"},
     * {type: "block", label: "Regular event", classname: "purple"},
     * {type: "spacer"},
     * {type: "text", label: "Bad"},
     * {type: "list", list: ["grade-1", "grade-2", "grade-3", "grade-4"]},
     * {type: "text", label: "Good"}
     * ]
     *
     * @param string $type     The display type. Values 'text', 'block', 'list' and 'spacer' are allowed.
     * @param string $label    Label. Required for display type 'text', optional for 'block'. Not used for 'list' or 'spacer'.
     * @param string $badge    Extra setting for display type 'text' to show badge information.
     * @param string $corFundo Extra setting for display type 'text' (applied to the badge) and 'block' to add a css class to the legend item.
     * @param array  $list     Array of css classnames for the list of blocks for type 'list'.
     *
     * @return $this
     */
    public function addLegenda ($type, $label = null, $badge = null, $corFundo = null, $list = null)
    {
        $this->legend[] = [
            'type'     => $type,
            'label'    => $label,
            'badge'    => $badge,
            'list'     => $list,
            'corFundo' => $corFundo
        ];

        return $this;
    }

    /**
     * Possibilita adicionar um array de legenda ja construido
     *
     * @param array $legenda
     * @return $this
     */
    public function setLegenda ($legenda)
    {
        $this->legend = simec_utf8_encode_recursive($legenda);

        return $this;
    }

    /**
     * @param string      $dataInicial (Y-m-d)
     * @param string      $dataFinal   (Y-m-d)
     * @param bool        $badge
     * @param null|string $titulo
     * @param null|string $corpo
     * @param null|string $rodape
     * @param string      $corFundo
     * @param bool        $modal
     *
     * @return $this
     * @throws Exception
     */
    public function addEventoPeriodo ($dataInicial, $dataFinal, $badge = false, $titulo = null, $corpo = null, $rodape = null, $corFundo, $modal = false)
    {
        $dInicio = new DateTime($dataInicial);
        $dDiff = $dInicio->diff(new DateTime($dataFinal));

        if ($dDiff->format('%R') == '-')
        {
            throw new Exception ('Data Inicial é maior que Data Final');
        }

        // adiciona o primeiro dia
        $this->addEventoUmDia($dInicio->format('Y-m-d'), $badge, $titulo, $corpo, $rodape, $corFundo, $modal);

        // adiciona os demais dias do intervalo
        for ($i = 0; $i < $dDiff->days; $i++)
        {
            $this->addEventoUmDia($dInicio->modify("+1 day")->format('Y-m-d'), $badge, $titulo, $corpo, $rodape, $corFundo, $modal);
        }

        return $this;
    }

    /**
     * @param string      $data (Y-m-d)
     * @param bool        $badge
     * @param null|string $titulo
     * @param null|string $corpo
     * @param null|string $rodape
     * @param string      $corFundo
     * @param bool        $modal
     *
     * @return $this
     */
    public function addEventoUmDia ($data, $badge = false, $titulo = null, $corpo = null, $rodape = null, $corFundo, $modal = false)
    {
        $dado = [
            'date'   => ($data),
            'badge'  => (bool) $badge,
            'title'  => ((empty($titulo) ? ' ' : $titulo)),
            'body'   => ($corpo),
            'footer' => ($rodape),
//            'classname' => simec_utf8_encode_recursive($corFundo),
            'modal'  => (bool) $modal
        ];

        $this->addGrupoCor($corFundo, $data);

        $this->data[] = array_filter($dado, 'self::filtroArray');

        return $this;
    }

    /**
     * Esta função adiciona um código na função executada quando o usuário altwera o mês do calendário.
     *
     * @param $script
     * @return $this
     */
    public function addScript ($script){
        $this->script = $script;
        return $this;
    }

    public function render ()
    {
        // gera id unico
        $id = time();

        $dados = $this->getTodosOsDados();

        // gera estilos css
        $css = $this->gerarCssDatas($dados);
        $css .= $this->gerarCssLegenda($dados);

        // conversao de JSON
        $json = json_encode($dados);

        $script = <<<HTML
<div id="calendario-dashboard-{$id}"></div>

<script src="/includes/zabuto_calendar/zabuto_calendar.min.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/zabuto_calendar/zabuto_calendar.min.css">

<style>
    .badge-today {
        padding: 7px !important;
        margin-top: -3px !important;
        margin-bottom: -4px !important;
    }

    .badge { border-radius: 40px !important; }
{$css}</style>
<script>
$(document).ready(function () {
    var dadosZabuto{$id} = {$json};

    // converte string em chamada de funcao
    dadosZabuto{$id}.action_nav = eval(dadosZabuto{$id}.action_nav);

    $("#calendario-dashboard-{$id}").zabuto_calendar(dadosZabuto{$id});

    // cria popover dos eventos
    carregazabutocalendar(500);
});

function carregazabutocalendar(tempo) {
    {$this->script}
    setTimeout(function () {
        $.each($(".zabuto_calendar"), function(index, item) {
            var zabuto = $(item);
            $.each(zabuto.parent().data()['jsonData'], function (index, item) {
                $('#' + zabuto.attr('id') + "_" + item.date + "_day").attr('data-html', 'true').attr('data-placement', 'top').attr('data-trigger', 'hover').attr('data-toggle', 'popover').attr('data-content', item.body);

                $('[data-toggle="popover"]').popover();
            });
        });
    }, tempo ? tempo : 100);
}
</script>
HTML;

        echo $script;
    }

    private function addGrupoCor ($corFundo, $data)
    {
        $this->grupoCores[$data][] = $corFundo;

        //  sort($this->grupoCores);
    }

    /**
     * Gera as classes CSS e setam os nomes da classe nas legendas
     *
     * @param $dados
     * @return string
     */
    private function gerarCssLegenda (&$dados)
    {
        $contador = 0;
        $idUnico = time();
        $classes = [];
        $css = '';

        foreach ($dados['legend'] as $index => &$legend)
        {
            if (empty($legend['corFundo']) && empty($legend['list']))
            {
                continue;
            }

            if (!empty($legend['corFundo']))
            {
                $contador++;
                $nomeClasse = "legend-{$contador}-{$idUnico}";

                $classes[] = [
                    'nome'       => $nomeClasse,
                    'background' => "background: {$legend['corFundo']}"
                ];

                $dados['legend'][$index]['classname'] = ($nomeClasse);
            }

            if (!empty($legend['list']))
            {
                foreach ($legend['list'] as $i => $cor)
                {
                    $nomeClasse = "legend-{$contador}-{$idUnico}";
                    $contador++;

                    $classes[] = [
                        'nome'       => $nomeClasse,
                        'background' => "background: {$cor}"
                    ];

                    // inserir os nomes das classes de css gerados dentro do array LEGEND
                    $legend['list'][$i] = $nomeClasse;
                }
            }
        }

        // escreve o estilo css
        foreach ($classes as $index => $class)
        {
            $css .= ".{$class['nome']} { {$class['background']} }";
        }

        return $css;
    }

    /**
     * Gera as classes CSS e setam os nomes das classes nas datas com eventos
     *
     * @param $dados
     * @return string
     */
    private function gerarCssDatas (&$dados)
    {
        $contador = 0;
        $idUnico = time();
        $classes = [];
        $css = '';

        foreach ($this->grupoCores as $data => $coresFundo)
        {
            $contador++;
            $nomeClasse = "grade-{$contador}-{$idUnico}";
            $qtdCores = count($coresFundo);
            if ($qtdCores > 1)
            {
                $classe = [
                    'nome'       => $nomeClasse,
                    'background' => 'background: linear-gradient(to bottom, '
                ];

                $porcentagem = 100 / $qtdCores;
                $somaPorcentagem = 0;
                $somaPorcentagemAux = $porcentagem;

                for ($i = 0; $i < $qtdCores; $i++)
                {
                    $classe['background'] .= "{$coresFundo[$i]} {$somaPorcentagem}%, {$coresFundo[$i]} {$somaPorcentagemAux}%, ";
                    $somaPorcentagem += $porcentagem;
                    $somaPorcentagemAux += $porcentagem;
                }

                $classe['background'] = rtrim($classe['background'], ', ') . ');';
            }
            else
            {
                $classe = [
                    'nome'       => $nomeClasse,
                    'background' => "background: {$coresFundo[0]}"
                ];
            }

            // Verifica se ja existe um css com a mesma cor
            $key = array_search($classe['background'], array_column($classes, 'background'));
            if ($key !== false)
            {
                $nomeClasse = $classes[$key]['nome'];
            }
            else
            {
                $classes[] = $classe;
            }

            // inserir os nomes das classes de css gerados dentro do array DATA
            foreach ($dados['data'] as $index => $datum)
            {
                if ($datum['date'] == $data)
                {
                    $dados['data'][$index]['classname'] = ($nomeClasse);
                }
            }
        }

        // escreve o estilo css
        foreach ($classes as $index => $class)
        {
            $css .= ".{$class['nome']} { {$class['background']} }\n";
        }

        return $css;
    }

    /**
     * @param array $dado
     * @return $this
     */
    private function setDados ($dados)
    {
        foreach ($dados as $index => $dado)
        {
            $this->addEventoUmDia(
                $dado['date'],
                $dado['badge'],
                $dado['title'],
                $dado['body'],
                $dado['footer'],
                $dado['classname'],
                $dado['modal']
            );
        }

        return $this;
    }

    private function getTodosOsDados ()
    {
        $mapa = [];
        foreach ($this->data as $index => $datum)
        {
            if (!key_exists($datum['date'], $mapa))
            {
                $mapa[$datum['date']] = $datum;
            }
            else
            {
                $titulo = trim($datum['title']);
                if (!empty($titulo))
                {
                    $mapa[$datum['date']]['title'] .= ("<br>{$titulo}");
                }
                if (isset($datum['body']))
                {
                    $mapa[$datum['date']]['body'] .= ("<br>{$datum['body']}");
                }
            }
        }

        $this->data = array_values($mapa);

        return array_filter([
            'cell_border'   => (bool) $this->cell_border,
            'today'         => (bool) $this->today,
            'show_days'     => (bool) $this->show_days,
            'weekstartson'  => $this->weekstartson,
            'year'          => $this->year,
            'month'         => $this->month,
            'show_previous' => $this->show_previous,
            'show_next'     => $this->show_next,
            'language'      => ($this->language),
            'legend'        => simec_utf8_encode_recursive($this->legend),
            'action'        => ($this->action),
            'action_nav'    => ($this->action_nav),
            'data'          => $this->data
        ], 'self::filtroArray');
    }
}