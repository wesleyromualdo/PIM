<?php
/**
 * Implementação do relatório do extrato dinamico.
 *
 * @version $Id: Relatorio.php 143900 2018-09-14 13:56:00Z victormachado $
 */

class Simec_ExtratoDinamico_Relatorio
{
    protected $config;
    protected $form;

    public function __construct(Simec_ExtratoDinamico_Config $config, Simec_ExtratoDinamico_Form $form)
    {
        $this->config = $config;
        $this->form = $form;
    }

    public function render()
    {
        if (!$this->getForm()->formularioSubmetido()) {
            return '';
        }

        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $list->setQuery($this->montaQuery())
            ->turnOnPesquisator()
            ->setCabecalho($this->montaCabecalho())
            ->setFormFiltros($this->getForm()->getId());

        foreach ($this->getForm()->getColunasSelecionadas() as $coluna) {
            if ($callback = $this->getConfig()->getCallbackColuna($coluna)) {
                $list->addCallbackDeCampo($coluna, $callback);
            }
        }

        if ($colunasTotalizadas = $this->getConfig()->getColunasTotalizadas()) {
            $list->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunasTotalizadas);
        }

        return $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * @return Simec_ExtratoDinamico_Config|Ted_ExtratoDinamico_Config
     */
    protected function getConfig()
    {
        return $this->config;
    }

    protected function getForm()
    {
        return $this->form;
    }

    protected function montaQuery()
    {
        $sql = <<<DML
SELECT {$this->montaQuerySelect()}
  FROM {$this->getConfig()->getDbView()}
  {$this->montaQueryWhere()}
  {$this->montaQueryGroupby()}
  {$this->montaQueryOrderby()}
DML;

        return $sql;
    }

    protected function montaQuerySelect()
    {
        if (!$this->getForm()->formularioSubmetido()) {
            return '*';
        }

        $colunas = [];
        foreach ($this->getForm()->getColunasSelecionadas() as $coluna) {
            $colunas[] = $this->getConfig()->getSelectColuna($coluna);
        }

        return implode(', ', $colunas);
    }

    protected function montaQueryWhere()
    {
        $where = [];
        foreach ($this->getForm()->getFiltrosSelecionados() as $filtro => $valores) {
            if (count($valores) > 1) {
                $where[] = "{$filtro} IN('" . implode("', '", $valores) . "')";
                continue;
            }
            $where[] = "{$filtro} = '" . current($valores) . "'";
        }

        if (empty($where)) {
            return '';
        }

        return 'WHERE ' . implode(' AND ', $where);
    }

    protected function montaQueryGroupby()
    {
        $qtdColQualitativas = count($this->getForm()->getColunasSelecionadas(Simec_ExtratoDinamico::QUALITATIVO));
        $qtdColQuantitativas = count($this->getForm()->getColunasSelecionadas(Simec_ExtratoDinamico::QUANTITATIVO));

        $groupby = '';
        if ($qtdColQuantitativas > 0 || !$this->form->mostracampoquantitativo) {
            $groupbyArr = array_keys(array_fill(1, $qtdColQualitativas, 1));
            if(!empty($groupbyArr) && is_array($groupbyArr)){
                $groupby = 'GROUP BY ' . implode(', ', $groupbyArr);
            }
        }

        return $groupby;
    }

    protected function montaQueryOrderby()
    {
        $qtdColunas = count($this->getForm()->getColunasSelecionadas());
        
        $orderby = '';
        if($qtdColunas > 0){
        	$orderby =  'ORDER BY ' . implode(', ', array_keys(array_fill(1, $qtdColunas, 1)));
        }
        
        return $orderby;
        
        
    }

    protected function montaCabecalho()
    {
        return $this->getConfig()->getTituloColuna(
            $this->getForm()->getColunasSelecionadas()
        );
    }
}
