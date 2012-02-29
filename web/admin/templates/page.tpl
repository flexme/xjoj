      <div id="turn-page">
        Total: <span id="totalRecords">{$record_count}</span>
        Pages: <span id="totalPages">{$page_count}</span>
        Page: <span id="pageCurrent">{$filter.page}</span>
        <span id="page-link">
          {literal}<a href="javascript:listTable.gotoPageFirst()">{/literal}First Page</a>
          {literal}<a href="javascript:listTable.gotoPagePrev()">{/literal}Prev Page</a>
          {literal}<a href="javascript:listTable.gotoPageNext()">{/literal}Next Page</a>
          {literal}<a href="javascript:listTable.gotoPageLast()">{/literal}Last Page</a>
          <select id="gotoPage" onchange="listTable.gotoPage(this.value)">
            {create_pages count=$page_count page=$filter.page}
          </select>
        </span>
      </div>
