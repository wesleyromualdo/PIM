<?php
/**
 * Arquivo de controle da carga da LOA.
 * @version $Id: subacoes.inc 136888 2018-01-24 22:34:07Z maykelbraz $
 */

/**
 * Cabeçalho simec
 */
require_once APPRAIZ . 'includes/cabecalho.inc';

function jobconfig()
{
    Spo_Job_Loa_Carregasubacoes
        ::setPredecessor('Spo_Job_Loa_Preparabase_Subacoes',
            [
                'exercicio' => $_SESSION['exercicio']
            ]
        );

    return Simec_Job_Manager::start(
        Spo_Job_Loa_Carregasubacoes::class,
        [
            'exercicio' => $_SESSION['exercicio']
        ]
    );
}

$render = (new Simec_Job_Render('jobconfig'))
    ->setTitle('Carga de Subações');

(new Simec_View_Breadcrumb())
    ->add('Carga da LOA')
    ->add('4) Subações')
    ->render();
?>
<div class="col-sm-offset-1 col-sm-10">
    <div class="panel panel-primary">
    <div class="panel-heading text-center">Carga de dados da LOA</div>
        <div class="panel-body" style="padding: 0;">
            <div class="row" style="border-bottom: 1px solid #CCC; padding: 15px;">
                <div class="col-sm-6 text-right" style="font-weight: bold">Exercício:</div>
                <div class="col-sm-6"><?=$_SESSION['exercicio'];?></div>
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
