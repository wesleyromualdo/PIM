<?php
/**
 * Arquivo de controle da carga da LOA.
 * @version $Id: execucaoorcamentaria.inc 136888 2018-01-24 22:34:07Z maykelbraz $
 */

/**
 * Cabeçalho simec
 */
require_once APPRAIZ . 'includes/cabecalho.inc';

define('MOMENTO_LOA_CREDITOS', 9000);

function jobconfig()
{
    Spo_Job_Loa_Quantitativo_Execucaoorcamentaria
        ::setPredecessor('Spo_Job_Loa_Preparabase_Execucaoorcamentaria');

    Spo_Job_Loa_Carregaptres::setPredecessor(
        Spo_Job_Loa_Quantitativo_Execucaoorcamentaria::class,
        [
            'exercicio' => $_SESSION['exercicio'],
            'momento' => MOMENTO_LOA_CREDITOS,
            'sisid' => $_SESSION['sisid']
        ]
    );

    return Simec_Job_Manager::start(
        Spo_Job_Loa_Carregaptres::class,
        [
            'exercicio' => $_SESSION['exercicio'],
            'momento' => MOMENTO_SPO,
            'sisid' => $_SESSION['sisid']
        ]
    );
}

$render = (new Simec_Job_Render('jobconfig'))
    ->setTitle('Carga da LOA');

(new Simec_View_Breadcrumb())
    ->add('Carga da LOA')
    ->add('2) PTRES')
    ->render();
?>
<div class="col-sm-offset-1 col-sm-10">
    <div class="panel panel-primary">
        <div class="panel-heading text-center">Carga de dados da LOA</div>
        <div class="panel-body" style="padding: 0;">
            <div class="row" style="border-bottom: 1px solid #CCC; padding: 15px;">
                <div class="col-sm-3 text-right" style="font-weight: bold">Momento:</div>
                <div class="col-sm-3"><?='Lei+Créditos (9000)';?></div>
                <div class="col-sm-3 text-right" style="font-weight: bold">Exercício:</div>
                <div class="col-sm-3"><?=$_SESSION['exercicio'];?></div>
            </div>
        </div>
    </div>
</div>
<?php
Simec_Job_Manager::render(
    null,
    null,
    $render
);
