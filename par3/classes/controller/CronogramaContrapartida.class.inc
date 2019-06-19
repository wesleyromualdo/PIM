<?php


class Par3_Controller_CronogramaContrapartida extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Obra();
    }

    public function salvar($dados){
        $dados['obrvalor_contrapartida'] = str_replace('.','',$dados['obrvalor_contrapartida']);
        $dados['obrvalor_contrapartida'] = str_replace(',','.',$dados['obrvalor_contrapartida']);
        $dados['obrvalor_contrapartida'] = $dados['obrvalor_contrapartida'] ? $dados['obrvalor_contrapartida'] : '0';
        $url = "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=cronograma_contrapartida&inuid={$dados['inuid']}&inpid={$_REQUEST['inpid']}&obrid={$dados['obrid']}";

        try{
            $this->model->popularDadosObjeto($dados);
            $id = $this->model->salvar();
            $this->model->commit();
            simec_redirecionar($url, 'success', 'Valor de contrapartida Salvo com sucesso!');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error', 'Erro ao salvar Valor de Contrapartida!');
        }

    }

    public function calculaValorPorcentagem($valor, $percentual){
        if($percentual < 100){
            $percentual = '0.' . $percentual;
        }else{
            $percentual = 1;
        }
        return $valor * $percentual;
    }

    public function buscaValorEmpenhadoObra($obrid){
        $sql = "SELECT valor_total_empenhado
        FROM par3.empenho emp
        INNER JOIN par3.empenhoobracomposicao eoc ON eoc.empid = emp.empid 
        INNER JOIN par3.processoobracomposicao poc ON poc.pocid = eoc.pocid
        WHERE poc.obrid = {$obrid}";
        return $this->model->pegaUm($sql);
    }

}