<?php

class ProgramaSecretaria_Model extends Modelo
{
    protected $stNomeTabela = 'corporativo.programa_secretaria';

    protected $arChavePrimaria = array(
        'co_programa_secretaria',
    );

    protected $arAtributos = array(
        'co_programa_secretaria' => null,
        'co_programa' => null,
        'co_secretaria' => null
    );

    protected $arChaveEstrangeira = array(
        'co_programa' => array('tabela' => 'corporativo.programa', 'pk' => 'co_programa'),
        'co_secretaria' => array('tabela' => 'corporativo.entidades_juridica', 'pk' => 'enjid'),
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'co_programa_secretaria' => array('Digits'),
            'co_programa' => array('Digits'),
            'co_secretaria' => array('Digits')
        );
    }

    public function limparParaId($co_programa)
    {
        $this->excluirVarios("co_programa = ".$co_programa);
        $this->commit();
    }

    public function inserirAssociacao($arr)
    {
        $values = "";
        foreach($arr as $key => $item)
        {
            $values .= "(".$item['co_programa'].",".$item['co_secretaria'].")".(isset($arr[$key+1]) ? "," : "");
        }

        $sql = "INSERT INTO corporativo.programa_secretaria (co_programa, co_secretaria) VALUES $values";

        $this->executar($sql);
        $this->commit();
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

}
